#ifndef SOURCEMETA_JSONTOOLKIT_JSON_H_
#define SOURCEMETA_JSONTOOLKIT_JSON_H_

#include <string>
#include <memory>

namespace sourcemeta {
  namespace jsontoolkit {

    class JSON {
      public:
        JSON(const std::string &json);
      private:
        // Hide the JSON backend
        // See https://en.cppreference.com/w/cpp/language/pimpl
        struct Backend;
        std::unique_ptr<Backend> backend;
    };

  }
}

#endif
