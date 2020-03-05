#ifndef GT_SWAPPED_ARG_CHECKER_H
#define GT_SWAPPED_ARG_CHECKER_H

#include "Compiler.hpp"

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// A description of a function being called.
class CallDeclDescriptor {
public:
  // The fully qualified name of the callee.
  std::string fullyQualifiedName;

  // The names of the formal parameters from the callee.
  std::optional<std::vector<std::string>> paramNames;

  std::optional<bool> isVariadicFunction;
  std::optional<bool> isMemberFunction;
};

// A single call-site to check for swapped argument errors.
class CallSite {
public:
  using ArgumentNames = std::vector<std::string>;

  // Details about the callee of the call site.
  CallDeclDescriptor callDecl;

  // Name expressions for each positional argument.
  std::vector<ArgumentNames> positionalArgNames;

  // Used for languages that support named arguments. Unused for checking C/C++.
  std::optional<std::map<std::string, ArgumentNames>> namedArgNames;

  // Used by the checker to do false-positive reduction. The callback
  // returns N lines relative to the call site location.
  std::optional<std::function<std::string(size_t, size_t)>> sourceFetchCallback;
  // TODO: name of function containing the call site
};

// A swapped argument error.
class Result {
public:
  using ArgumentIndex = std::variant<size_t, std::string>;

  // Indices of the swapped arguments.
  ArgumentIndex arg1;
  ArgumentIndex arg2;

  // The checker's confidence in this being a true positive, 0-100. Tools
  // can map this value to be in their "native" range.
  double score;

  std::string debugStr() const;
};

class SWAPPED_ARG_EXPORT Checker {
public:
  explicit Checker(
      std::string modelPath = "" /*TODO: additional configuration stuff */);

  // Checks for all argument swap errors at a given call site.
  // @param site Details about the call site.
  // @param report_callback The callback to call for each swapped argument
  //                        problem.
  void CheckSite(const CallSite& site,
                 std::function<void(const Result&)> reportCallback);
};

#endif // GT_SWAPPED_ARG_CHECKER_H
