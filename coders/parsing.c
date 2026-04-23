/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 15:11:23 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/19 15:27:00 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (atoi(str) <= 0)
		return (0);
	return (1);
}

int	is_valid_scheduler(char *str)
{
	if (!str || str[0] == '\0')
		return (0);
	if (strcmp(str, "fifo") == 0 || strcmp(str, "edf") == 0)
		return (1);
	return (0);
}
