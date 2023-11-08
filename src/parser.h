#ifndef BRAINIAC_PARSER_H
#define BRAINIAC_PARSER_H

#include "node.h"
#include "scanner.h"

// Parse a program from a scanner.
Node *parseScanner(Scanner *scanner);

#endif // BRAINIAC_PARSER_H
