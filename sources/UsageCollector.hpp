// Copyright 2012 by Laszlo Nagy [see file MIT-LICENSE]

#ifndef _UsageCollector_hpp_
#define _UsageCollector_hpp_

#include "ScopeAnalysis.hpp"

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>


// Collect variable usages. One variable could have been used multiple
// times with different constness of the given type.
class UsageCollector {
protected:
    UsageCollector(ScopeAnalysis::UsageRefsMap & Out);
    virtual ~UsageCollector();

    void AddToResults(
            clang::Expr const * const Stmt,
            clang::QualType const & Type = clang::QualType());

    void Report(char const * const Message, clang::DiagnosticsEngine &) const;

private:
    ScopeAnalysis::UsageRefsMap & Results;
};

#endif // _UsageCollector_hpp_
