#include "minishell.h"

int	is_in_set(char const s1, char const *set)
{
	while (*set)
	{
		if (s1 == *set)
			return (1);
		set++;
	}
	return (0);
}

int	end_trim(char const *s1, char const *set, size_t i)
{
	while (i > 0)
	{
		i--;
		if (is_in_set(s1[i], set) == 0)
			return (i + 1);
	}
	return (i + 1);
}

int	start_trim(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i])
	{
		if (is_in_set(s1[i], set) == 0)
			return (i);
		i++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	size_t	j;
	char	*new;

	if (!s1 || !set)
		return (NULL);
	i = start_trim(s1, set);
	len = ft_strlen(s1);
	if (i != len)
		len = end_trim(s1, set, len);
	new = (char *)malloc(sizeof(char) * (len - i + 1));
	if (new != NULL)
	{
		j = 0;
		while (i < len)
		{
			new[j] = s1[i];
			j++;
			i++;
		}
		new[j] = '\0';
	}
	return (new);
}