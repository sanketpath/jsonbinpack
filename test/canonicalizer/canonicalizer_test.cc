#include <gtest/gtest.h>
#include <jsonbinpack/canonicalizer/canonicalizer.h>
#include <jsontoolkit/json.h>

#include <cassert>    // assert
#include <filesystem> // std::filesystem::path
#include <fstream>    // std::ifstream
#include <string>     // std::string

// Heavily inspired by https://stackoverflow.com/a/116220
static auto read_file(const std::filesystem::path path) -> std::string {
  constexpr std::size_t buffer_size{4096};
  std::ifstream stream{path.string()};
  stream.exceptions(std::ios_base::badbit);
  std::string buffer(buffer_size, '\0');
  std::string output;
  while (stream.read(&buffer[0], buffer_size)) {
    output.append(buffer, 0,
                  static_cast<std::string::size_type>(stream.gcount()));
  }
  return output.append(buffer, 0,
                       static_cast<std::string::size_type>(stream.gcount()));
}

class CanonicalizerTest : public testing::Test {
public:
  explicit CanonicalizerTest(
      const sourcemeta::jsontoolkit::JSON<std::string> &document,
      const sourcemeta::jsontoolkit::JSON<std::string> &expected)
      : document{document}, expected{expected} {}

  void TestBody() override {
    sourcemeta::jsonbinpack::canonicalize(this->document);
    this->document.parse();
    this->expected.parse();
    EXPECT_EQ(this->expected, this->document);
  }

private:
  sourcemeta::jsontoolkit::JSON<std::string> document;
  sourcemeta::jsontoolkit::JSON<std::string> expected;
};

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  for (const auto &test_file :
       {"misc.json", "number.json", "array.json", "object.json", "string.json",
        "boolean.json", "null.json"}) {
    const std::filesystem::path test_path =
        std::filesystem::path{TEST_BASE_DIRECTORY} / test_file;
    const std::string raw_test_document{read_file(test_path)};
    sourcemeta::jsontoolkit::JSON<std::string> test_document{raw_test_document};
    test_document.parse();
    assert(test_document.is_array());

    const std::string test_category =
        std::string{"Canonicalizer."} +
        std::filesystem::path{test_file}.stem().string();
    for (const auto &test_case : test_document.to_array()) {
      assert(test_case.defines("title"));
      assert(test_case.at("title").is_string());
      assert(test_case.defines("document"));
      assert(test_case.defines("expected"));
      const std::string title{test_case.at("title").to_string()};
      testing::RegisterTest(
          test_category.c_str(), title.c_str(), nullptr, nullptr, __FILE__,
          __LINE__, [=]() -> CanonicalizerTest * {
            return new CanonicalizerTest(test_case.at("document"),
                                         test_case.at("expected"));
          });
    }
  }

  return RUN_ALL_TESTS();
}
