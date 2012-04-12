// Copyright 2011 by Laszlo Nagy [see file LICENSE.TXT]

#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/AST.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <boost/bind.hpp>

namespace
{

class ASTConsumer : public clang::ASTConsumer
{
public:
    ASTConsumer()
    { }

    static bool is_builtin(clang::NamedDecl const * decl)
    {
        clang::IdentifierInfo const * const id = decl->getIdentifier();
        return (id) && (id->isStr("__va_list_tag") || id->isStr("__builtin_va_list"));
    }

    virtual bool HandleTopLevelDecl(clang::DeclGroupRef gd)
    {
        for (clang::DeclGroupRef::iterator it = gd.begin(), e = gd.end(); it != e; ++it)
        {
            if (clang::NamedDecl const * const current = clang::dyn_cast<clang::NamedDecl>(*it))
            {
                if (! is_builtin(current))
                {
                    clang::PrintingPolicy pp(current->getASTContext().getLangOpts());
                    pp.Dump = true;

                    current->print(llvm::errs(), pp);
                }
            }
        }
        return true;
    }
};

class FindConstCandidateAction : public clang::PluginASTAction
{
protected:
    clang::ASTConsumer * CreateASTConsumer(clang::CompilerInstance & compiler, llvm::StringRef)
    {
        return new ASTConsumer();
    }

    bool ParseArgs(clang::CompilerInstance const & compiler,
                   std::vector<std::string> const & args)
    {
        for (unsigned i = 0, e = args.size(); i != e; ++i)
        {
            llvm::errs() << "PrintFunctionNames arg = " << args[i] << "\n";
            // Example error handling.
            if (args[i] == "-an-error")
            {
                clang::DiagnosticsEngine & D = compiler.getDiagnostics();
                unsigned DiagID = D.getCustomDiagID(
                                      clang::DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
                D.Report(DiagID);
                return false;
            }
        }
        if (args.size() && args[0] == "help")
        {
            PrintHelp(llvm::errs() );
        }
        return true;
    }
    void PrintHelp(llvm::raw_ostream & ros)
    {
        ros << "Help for PrintFunctionNames plugin goes here\n";
    }
};

}

static clang::FrontendPluginRegistry::Add<FindConstCandidateAction>
    x("find-const-candidate", "suggests declaration to be const");
