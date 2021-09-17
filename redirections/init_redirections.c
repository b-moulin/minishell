/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:53:28 by bmoulin           #+#    #+#             */
/*   Updated: 2021/08/26 11:53:31 by bmoulin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_redirection_gauche(t_list **lst, t_red_gauche	*rg)
{
	rg->new = *lst;
	rg->count = 0;
	rg->name = 0;
	rg->ret_fd = 1;
	rg->red_type[0] = &right;
	rg->red_type[1] = &double_right;
	rg->red_type[2] = &left;
	rg->red_type[3] = &double_left;
}

int	loop_redirection_gauche(t_shell *shell, t_red_gauche *rg)
{
	rg->new->lst_struct->redir = rg->new->lst_struct->redir->next;
	rg->name = rg->new->lst_struct->redir->content.word;
	if (rg->red == e_right || rg->red == e_double_right)
	{
		rg->ret_fd = rg->red_type[rg->red](rg->name);
		if (rg->ret_fd == -1)
			return (-1);
	}
	else
	{
		shell->read_fd = rg->red_type[rg->red](rg->name);
		if (shell->read_fd == -1)
			return (-1);
	}
	return (0);
}

void	firstpart_redirection_gauche(t_red_gauche *rg)
{
	if (rg->count % 2 == 0 && rg->new->lst_struct->redir->content.word)
	{
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '>'
			&& rg->new->lst_struct->redir->content.word[1] == 0)
			rg->red = e_right;
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '>'
			&& rg->new->lst_struct->redir->content.word[1]
			&& rg->new->lst_struct->redir->content.word[1] == '>'
			&& rg->new->lst_struct->redir->content.word[2] == 0)
			rg->red = e_double_right;
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '<'
			&& rg->new->lst_struct->redir->content.word[1] == 0)
			rg->red = e_left;
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '<'
			&& rg->new->lst_struct->redir->content.word[1]
			&& rg->new->lst_struct->redir->content.word[1] == '<'
			&& rg->new->lst_struct->redir->content.word[2] == 0)
			rg->red = e_double_left;
	}
}

int	redirection_gauche(t_list **lst, t_shell *shell)
{
	t_red_gauche	*rg;
	int				ret;

	rg = malloc(sizeof(t_red_gauche));
	if (!rg)
		exit(0);
	init_redirection_gauche(lst, rg);
	while (rg->new && rg->new->lst_struct && rg->new->lst_struct->redir)
	{
		firstpart_redirection_gauche(rg);
		if (loop_redirection_gauche(shell, rg) != 0)
		{
			free(rg->name);
			free(rg);
			return (-1);
		}
		rg->count++;
		if (rg->new->lst_struct->redir)
			rg->new->lst_struct->redir = rg->new->lst_struct->redir->next;
	}
	ret = rg->ret_fd;
	free(rg->name);
	free(rg);
	return (ret);
}

int	get_fd(t_list **lst, t_shell *shell)
{
	t_list	*new;
	t_list	*first_redir;
	int		fd;

	new = *lst;
	while (new)
	{
		first_redir = new->lst_struct->redir;
		while (new->lst_struct->redir)
			new->lst_struct->redir = new->lst_struct->redir->next;
		new->lst_struct->redir = first_redir;
		new = new->next;
	}
	new = *lst;
	fd = redirection_gauche(lst, shell);
	return (fd);
}
