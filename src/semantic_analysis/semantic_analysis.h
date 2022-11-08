/*
    Header file containing all functions related to semantic analysis
*/
#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include "ast.h"
#include <stdbool.h>

bool name_analysis(AST_func_list *ast);

#endif /* SEMANTIC_ANALYSIS_H */