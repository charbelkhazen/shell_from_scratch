#include "minishell.h"

static void indent(int depth)
{
    while (depth--) putchar(' ');
}

static void printtree_rec(const t_tree *tree, int depth)
{
    if (!tree) return;

    switch (tree->type)
    {
    case 'w': {                     /* simple command            */
        const t_cmdtree *cmd = (const t_cmdtree *)tree;
        indent(depth);
        printf("CMD");
        for (int i = 0; cmd->cmd[i]; ++i)
            printf(" %s", cmd->cmd[i]);
        putchar('\n');
        break;
    }
    case '|': {                     /* pipeline                  */
        const t_pipetree *p = (const t_pipetree *)tree;
        indent(depth);
        puts("PIPE");
        printtree_rec(p->left,  depth + 2);
        printtree_rec(p->right, depth + 2);
        break;
    }
    case '>': case '<': case 'a':   /* >  <  >> (append) << (heredoc) */
    case 'h': {                     /* assume 'h' = heredoc id   */
        const t_redirtree *r = (const t_redirtree *)tree;
        indent(depth);
        printf("REDIR(%c) %s\n", tree->type, r->file_name);
        printtree_rec(r->cmd, depth + 2);
        break;
    }
    default:
        indent(depth);
        printf("UNKNOWN(%d)\n", tree->type);
    }
}

