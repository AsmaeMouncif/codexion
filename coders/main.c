/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 15:11:13 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/19 15:29:14 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	main(int ac, char **av)
{
	int		i;

	if (ac != 9)
	{
		write(2, "Error: invalid number of arguments\n", 35);
		return (1);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_valid_number(av[i]))
		{
			write(2, "Error: invalid numeric argument\n", 32);
			return (1);
		}
		i++;
	}
	if (!is_valid_scheduler(av[8]))
	{
		write(2, "Error: invalid scheduler\n", 25);
		return (1);
	}
	return (0);
}
