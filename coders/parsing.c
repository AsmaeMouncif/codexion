#include "codexion.h"

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[0] == '\0')
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
	if (str == NULL || str[0] == '\0')
		return (0);
	if (strcmp(str, "fifo") == 0 || strcmp(str, "edf") == 0)
		return (1);
	return (0);
}
