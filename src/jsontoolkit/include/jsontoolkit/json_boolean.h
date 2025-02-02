#ifndef SOURCEMETA_JSONTOOLKIT_JSON_BOOLEAN_H_
#define SOURCEMETA_JSONTOOLKIT_JSON_BOOLEAN_H_

#include <istream>     // std::istream
#include <ostream>     // std::ostream
#include <string>      // std::string
#include <string_view> // std::string_view

namespace sourcemeta::jsontoolkit::Boolean {

constexpr std::string_view token_constant_true{"true"};
constexpr std::string_view token_constant_false{"false"};
auto stringify(std::ostream &output, bool value) -> void;
auto parse(std::istream &input) -> bool;

} // namespace sourcemeta::jsontoolkit::Boolean

#endif
