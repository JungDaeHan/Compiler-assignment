/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash table */
static ScopeList scope_hash[SIZE];


/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

ScopeList getParent(char * scope)
{
    int i;
    char * tmp;
    ScopeList S;
    if (strcmp(scope, "") == 0 )
        return NULL;
     tmp = (char*)malloc(sizeof(char) * (strlen(scope) + 1));
     strcpy(tmp,scope);
     for(i = strlen(tmp);i>= 0; i--)
     {
         if (tmp[i] == ':')
         { tmp[i] = '\0';
           break;
         }
         else
             tmp[i] = '\0';
     }
     S = scope_hash[hash(tmp)];
     while ((S != NULL) && (strcmp(tmp,S->name) != 0))
         S = S->next;

     if (S == NULL)
         S = getParent(tmp);
     free(tmp);
     return S;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert_B(ScopeList S, char* name, char* type, int lineno, int loc,int arrayCheck)
{
  int h1=hash(name);
  BucketList l;
  l =  S->bucket[h1];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { 
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->type = type;
    l->lines->next = NULL;
    l->next = S->bucket[h1];
    l->arrayCheck = arrayCheck;
    S->bucket[h1] = l; }
  else /* found in table, so just add line number */
  { 
    LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
}   
void st_insert( char * scope, char * name, char * type, int lineno, int loc,int arrayCheck)
{
  int h1 = hash(scope);
  ScopeList S2 = scope_hash[h1];
  while((S2 != NULL) && (strcmp(scope,S2->name) != 0))
      S2 = S2->next;
  if(S2 == NULL)
  {
  S2 = (ScopeList) malloc(sizeof(struct ScopeListRec));
  memset(S2, 0, sizeof(struct ScopeListRec));
  S2->parent = getParent(scope);
  S2->name = (char *)malloc(sizeof(char) * (strlen(scope) +1));
  strcpy(S2->name, scope);
  S2->next = scope_hash[h1];
  scope_hash[h1] = S2;
  }
  st_insert_B(S2,name,type,lineno,loc,arrayCheck);
} /* st_insert */

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */

BucketList st_lookup_B(ScopeList S, char * name)
{
    int h1 = hash(name);
    if ( S == NULL)
        return NULL;
    BucketList L = S->bucket[h1];
    while(( L != NULL) && (strcmp(name,L->name) != 0))
        L = L->next;
    if(L == NULL) return NULL;
    else return L;
}

BucketList st_lookup ( char * scope, char* name )//스코프 이름이 같은건 어떡하지
{ 
  BucketList B;
  int h1 = hash(scope);
  ScopeList S2 = scope_hash[h1];

  while((S2 != NULL) && (strcmp(scope,S2->name) != 0))
      S2 = S2->next;
  if( S2 == NULL)
  {
      ScopeList parent_S = getParent(scope);
      if(parent_S == NULL)
          return NULL;
      return st_lookup(parent_S->name,name);
  }
  
  B = st_lookup_B(S2,name);
  if(B == NULL)
  {
      ScopeList parent_S = getParent(scope);
      if(parent_S == NULL)
          return NULL;
      return st_lookup(parent_S->name,name);
  }
  else return B;
}

BucketList st_lookup_excluding_parent ( char * scope, char * name)
{  
   int h1 = hash(scope);
   ScopeList S2 = scope_hash[h1];
   while((S2 != NULL) && (strcmp(S2->name, scope) != 0))
       S2 = S2->next;
   
   if(S2==NULL)
       return NULL;

   return st_lookup_B(S2,name);

}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */

int arrayCheck(char *scope, char *name)
{
    BucketList B = st_lookup(scope,name);
    if (B == NULL)
        return -1;
    return B->arrayCheck;
}

void linePlus(char * scope, char* name, int lineno)
{
    BucketList B;
    LineList L;
    ScopeList S = scope_hash[hash(scope)];
    while ((S != NULL) && (strcmp(scope, S->name) != 0))
      S = S->next;
    if (S == NULL)
      S = getParent(scope);
    while(S)
    {
        B = S->bucket[hash(name)];
        while ((B != NULL) && (strcmp(name, B->name) != 0))
            B = B->next;
        if(B == NULL)
            S = S->parent;
        else
            break;
    }
    L = B->lines;
    while(L->next != NULL) L = L->next;
    L->next= (LineList) malloc(sizeof(struct LineListRec));
    L->next->lineno = lineno;
    L->next->next = NULL;
}

void printSymTab_B(FILE * listing, ScopeList S)
{
  int i;
  fprintf(listing, "Variable Name Variable Type Location Line Numbers\n");
  fprintf(listing, "------------- ------------- -------- ------------\n");
  for (i=0;i<SIZE;++i)
  {
    if (S->bucket[i] != NULL)
    { BucketList l = S->bucket[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14s ",l->name);
        fprintf(listing,"%-14s ",l->type);
        fprintf(listing,"%-8d  ",l->memloc);
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
}

void printSymTab(FILE * listing)
{ 
  int i;
  for (i=0;i<SIZE;++i)
  { if (scope_hash[i] != NULL)
    { ScopeList S = scope_hash[i];
      while(S != NULL)
      {
        fprintf(listing,"Scope name : %s\n", S->name);
        printSymTab_B(listing, S);
        fprintf(listing,"\n\n\n");
        S = S->next;
      } /* printSymTab */
    }  
  }
}
