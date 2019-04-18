/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#define SIZE 211
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */

typedef struct LineListRec
{  int lineno;
   struct LineListRec * next;
} * LineList; 

typedef struct BucketListRec
{  char * name;
   char * type;
   LineList lines;
   int memloc;
   int arrayCheck;
   struct BucketListRec * next;
}  * BucketList;

typedef struct ScopeListRec
{  char * name;
   BucketList bucket[SIZE];
   struct ScopeListRec * parent;
   struct ScopeListRec * next;
   int location;
} * ScopeList;

void st_insert( char * scope, char * name, char* type, int lineno, int loc,int arrayCheck );

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */

BucketList st_lookup( char *scope, char * name);
BucketList st_lookup_excluding_parent( char * scope, char * name);
int arrayCheck(char * scope, char *name);

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif
