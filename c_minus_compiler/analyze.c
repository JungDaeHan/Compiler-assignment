/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

/* counter for variable memory locations */
static int location = 0;
static char * scope = "~";
/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */

char * make_Scope(TreeNode *t)
{
    char *string = NULL;
    if(t->nodekind == StmtK)
    { if(t->kind.stmt == FunK)
      { string = (char *) malloc(sizeof(char) * (strlen(scope) + strlen(t->attr.name) +3));
        sprintf(string, "%s:%s", scope, t->attr.name);
      }
      else if(t->kind.stmt == CompK)
      { 
        //fprintf(listing,"----%d--%s--\n", t->lineno,scope);  
        string = (char*)malloc(sizeof(char) * (strlen(scope) + 12));
        sprintf(string, "%s:%d\0", scope, t->lineno);
      }
    }
    if(string ==NULL)
    { string = (char *)malloc(sizeof(char) * (strlen(scope) + 2));
      strcpy(string, scope);
      //fprintf(listing,"머머머머 %s 머머머멈\n", scope);
    }
    return string;
}

static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  {  
    preProc(t);
    { int i;
      char * saveScope = scope;
      int saveloc = location;
      scope = make_Scope(t);
      location = 0;

      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    //fprintf(listing,"-----%s-----\n", t->attr.name);

      free(scope);
      scope = saveScope;
      location = saveloc;
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ int arrayCheck_n = 0;
  switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case FunK:
          if(st_lookup(scope,t->attr.name) == NULL)
             st_insert(scope,t->attr.name,t->attr.type,t->lineno,location++, arrayCheck_n);
          else
            fprintf(listing, "scope already existed\n");
          break;
        case arrValDecK:
          arrayCheck_n = 1;
        case ValDecK:
        case ParamK:
        case arrParamK:
          //fprintf(listing,"111111%s111111", scope);
          if (t->attr.name != NULL && st_lookup_excluding_parent(scope,t->attr.name) == NULL)
          {
              st_insert(scope,t->attr.name,t->attr.type,t->lineno,location++,arrayCheck_n);
              if(t->kind.stmt == arrValDecK)
                  location += t->attr.val -1;
          }
          else if(t->attr.name != NULL)
            fprintf(listing,"%s already existed\n", t->attr.name);
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      {
        case IdK:
        case arrIdK:
        case CallK:
          //fprintf(listing,"scope : %s\n name : %s\n",scope,t->attr.name);
          if(st_lookup(scope,t->attr.name) == NULL)
              fprintf(listing, "%s is not declared\n", t->attr.name);
          else
          { 
            if(arrayCheck(scope, t->attr.name) ==1)
                t->kind.exp = arrIdK;
            linePlus(scope,t->attr.name,t->lineno,0);
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void BuiltinFunctions(TreeNode ** t)
{
    TreeNode *input = newStmtNode(FunK);
    input->sibling = *t;
    *t = input;
    input->lineno = 0;
    input->attr.name = malloc(sizeof(char) * (strlen("input") +1 ));
    strcpy(input->attr.name, "input");
    input->attr.type = "int";

    TreeNode *output = newStmtNode(FunK);
    output->sibling = *t;
    *t = output;
    output->lineno = 0;
    output->attr.name = malloc(sizeof(char) * (strlen("output") +1 ));
    strcpy(output->attr.name, "output");
    output->attr.type = "void";

    TreeNode *param = newExpNode(ParamK);
    param->attr.name = "int";
    param->attr.name = malloc(sizeof(char) * (strlen("arg") + 1));
    strcpy(param->attr.name, "arg");
    param->lineno = 0;

    output->child[0] = param;
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ BuiltinFunctions(&syntaxTree);
  traverse(syntaxTree,insertNode,nullProc);
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d:%s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { case ExpK:
      switch(t->kind.exp)
      { 
          case OpK:
          case ConstK:
          case arrIdK:
          case IdK:
          case CallK:
            break;
          default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { BucketList B;
        char *tmp;
        case AssignK:
          if(t->child[0]->kind.exp == IdK || t->child[0]->kind.exp == CallK || t->child[0]->kind.exp== arrIdK)
          {  B = st_lookup(scope, t->child[0]->attr.name);
             if(B==NULL)
             {
                 typeError(t->child[0], "no declaration");
                 break;
             }
             if(strcmp(B->type,"int") != 0)
                 typeError(t->child[0], "it is not a integer type");
          }
          else if(strcmp(t->child[0]->attr.type,"int")!=0)
             typeError(t->child[0], "it is not a integer type");
          break;
        case ValDecK:
        case arrValDecK:
          if (strcmp(t->attr.type,"void") == 0)
              typeError(t, "variable can't be void type");
          break;
        case ReturnK:
          tmp = (char *)malloc(sizeof(char) * (strlen(scope) +1));
          strcpy(tmp, scope);
          strtok(tmp, ":");
          char *func = strtok(NULL, ":");
          B = st_lookup("~", func);
          if (B == NULL)
          {  char *tmp;
             tmp = (char *)malloc(sizeof(char) * (strlen(func) + strlen("there is no function name : ") +1 ));
             sprintf(tmp, "%s%s", "there is no function name : %s", func);
             typeError(t,tmp);
             free(tmp);
          }
          else if( strcmp(B->type,"int") !=0 )
              typeError(t, "Void function not returns a value");
          free(tmp);
          break;
        case IfK:
        case WriteK:
        case RepeatK:
        case FunK:
        case CompK:
        case WhileK:
          break;
        default:
          break;
      }
      break;
    default:
      break;

  }
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{ traverse(syntaxTree,nullProc,checkNode);
}
