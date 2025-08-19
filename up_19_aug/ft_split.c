/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jissa <jissa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:20:02 by jissa             #+#    #+#             */
/*   Updated: 2025/08/06 12:24:58 by jissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_words_count(const char *s, char c)
{
	int	k;
	int	words_count;

	k = 0;
	words_count = 0;
	while (s[k])
	{
		while (s[k] && s[k] == c)
			k++;
		if (s[k])
		{
			words_count++;
			while (s[k] && s[k] != c)
				k++;
		}
	}
	return (words_count);
}

static void	free_all(char **arr, int count)
{
	while (--count >= 0)
		free(arr[count]);
	free(arr);
}

static char	*alloc_word(const char *s, int start, int end)
{
	char	*word;
	int		len;
	int		i;

	len = end - start;
	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[start + i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**allocate_words(char const *s, char c,
							char **outer_arr, int words_count)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (s[j] && i < words_count)
	{
		while (s[j] && s[j] == c)
			j++;
		start = j;
		while (s[j] && s[j] != c)
			j++;
		if (start < j)
		{
			outer_arr[i] = alloc_word(s, start, j);
			if (!outer_arr[i])
				return (free_all(outer_arr, i), NULL);
			i++;
		}
	}
	outer_arr[i] = NULL;
	return (outer_arr);
}

char	**ft_split(char const *s, char c)
{
	int		words_count;
	char	**outer_arr;

	if (!s)
		return (NULL);
	words_count = get_words_count(s, c);
	outer_arr = (char **)malloc((words_count + 1) * sizeof(char *));
	if (!outer_arr)
		return (NULL);
	return (allocate_words(s, c, outer_arr, words_count));
}
