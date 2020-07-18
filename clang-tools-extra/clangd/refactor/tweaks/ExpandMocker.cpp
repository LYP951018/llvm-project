#include "refactor/Tweak.h"

#include "XRefs.h"
#include "support/Logger.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/LLVM.h"
#include "llvm/ADT/None.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Error.h"
#include <clang/AST/ExprCXX.h>
#include <AST.h>
#include <climits>
#include <memory>
#include <string>
#include <fmt/format.h>

namespace clang {
namespace clangd {
namespace {

class ExpandMocker : public Tweak {
  const char *id() const final;
  Intent intent() const override { return Intent::Refactor; }
  bool prepare(const Selection &Inputs) override;
  Expected<Effect> apply(const Selection &Inputs) override;
  std::string title() const override;

private:
  /// Cache the AutoTypeLoc, so that we do not need to search twice.
    llvm::Optional<std::string> subsitutedString;
    SourceRange sourceRange;
   // TypeSourceInfo CachedTypeSourceInfo;
  /// Create an error message with filename and line number in it
  llvm::Error createErrorMessage(const std::string &Message,
                                 const Selection &Inputs);
};

REGISTER_TWEAK(ExpandMocker)

std::string ExpandMocker::title() const { return "Expand the mocker"; }


bool ExpandMocker::prepare(const Selection& Inputs) {
  //CachedLocation = llvm::None;
  if (auto *Node = Inputs.ASTSelection.commonAncestor()) {
    const TypeTraitExpr *typeTraitExpr = Node->ASTNode.get<TypeTraitExpr>();
    if (typeTraitExpr != nullptr &&
        typeTraitExpr->getTrait() == TypeTrait::UTT_FMock) {
        if (typeTraitExpr->getNumArgs() == 0)
        {
            return false;
        }
        const TypeSourceInfo* sourceInfo = typeTraitExpr->getArg(0);
        const QualType qualType = sourceInfo->getType()->getAs<DecltypeType>()->getUnderlyingType();
        auto ptr = qualType.getTypePtr();
        auto type = ptr->getTypeClass();
        std::string paramString;
        std::vector<std::string> paramStrs;
        std::vector<std::string> underlineStrs;
        std::string retTypeStr;
        if (const FunctionProtoType* functionType = qualType.getTypePtr()->getAs<FunctionProtoType>())
        {
            for (const auto& param : functionType->param_types())
            {
                paramStrs.push_back(param.getAsString());
                underlineStrs.push_back("_");
            }
            retTypeStr = functionType->getReturnType().getAsString();
        }

        const std::string paramStringJoined = llvm::join(paramStrs, ", ");
        const std::string underlineStringJoined = llvm::join(underlineStrs, ", ");
        const std::string mockedFunctionName = std::string{ Lexer::getSourceText(
            CharSourceRange::getTokenRange(sourceInfo->getType()
                                               ->getAs<DecltypeType>()
                                               ->getUnderlyingExpr()
                                               ->getSourceRange()),
            Inputs.AST->getASTContext().getSourceManager(),
            Inputs.AST->getASTContext().getLangOpts()) };
        // CachedLocation = typeTraitExpr->getSourceRange();
        // CachedTypeSourceInfo = *sourceInfo;
        subsitutedString = fmt::format(
            R"(EXPECT_CALL(*CREATE_MOCKER(&{}), 
                MOCK_FUNCTION({}))
            .WillOnce([=]({}) -> {} {{
                return {{}};
             }});)",
            mockedFunctionName, underlineStringJoined, paramStringJoined, retTypeStr);
        sourceRange = typeTraitExpr->getSourceRange();
        return true;
    }
  }
  return false;
}

Expected<Tweak::Effect> ExpandMocker::apply(const Selection& Inputs) {
    auto& SrcMgr = Inputs.AST->getSourceManager();

    std::string PrettyTypeName = *subsitutedString;

    tooling::Replacement
        Expansion(SrcMgr, CharSourceRange(sourceRange, true),
            PrettyTypeName);

    return Effect::mainFileEdit(SrcMgr, tooling::Replacements(Expansion));
}

llvm::Error ExpandMocker::createErrorMessage(const std::string& Message,
    const Selection& Inputs) {
    auto& SrcMgr = Inputs.AST->getSourceManager();
    std::string ErrorMessage =
        Message + ": " +
        SrcMgr.getFilename(Inputs.Cursor).str() + " Line " +
        std::to_string(SrcMgr.getExpansionLineNumber(Inputs.Cursor));

    return llvm::createStringError(llvm::inconvertibleErrorCode(),
        ErrorMessage.c_str());
}
} // namespace
} // namespace clangd
} // namespace clang