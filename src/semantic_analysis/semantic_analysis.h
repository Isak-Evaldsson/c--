/*
    Header file containing all functions related to semantic analysis
*/
#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include "ast.h"
#include <stdbool.h>

bool name_analysis(AST_root *ast);

#endif /* SEMANTIC_ANALYSIS_H */