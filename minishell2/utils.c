#include "minishell.h"

size_t	len_int(int n)
{
	size_t	i;
	long	nb;

	nb = n;
	i = 1;
	if (nb == 2147483648)
		return (10);
	if (nb == -2147483648)
		return (11);
	if (nb < 0)
	{
		nb *= -1;
		i++;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	i;
	long	nb;

	nb = n;
	i = len_int(nb);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	str[i--] = '\0';
	if (nb == 0)
		str[0] = 48;
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	while (nb > 0)
	{
		str[i] = 48 + (nb % 10);
		nb = nb / 10;
		i--;
	}
	return (str);
}

void free_prompt(s_cmd *prompt)
{
    int i;

    i = 0;
    while(prompt[i].cmd)
    {
        free(prompt[i].cmd);
        i++;
    }

}

void free_token(s_token *token)
{
    int i;

    i = 0;
    while(token[i].token)
    {
        token[i].token = 0;
        token[i].type = 0;
        i++;
    }
}

void free_tab(char **tabl)
{
    int i;

    i = 0;
    while(tabl[i])
    {
        free(tabl[i]);
        i++;
    }
    free(tabl);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static char	*ft_malloc(char const *s, unsigned int start, size_t len)
{
	char	*s2;

	if (ft_strlen(s) == 1 && start > ft_strlen(s))
		s2 = (char *)malloc(sizeof(char));
	else if (len > ft_strlen(s))
		s2 = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	else if (start > ft_strlen(s))
		s2 = (char *)malloc(sizeof(char));
	else if (len == ft_strlen(s) && start < len && len != 0)
		s2 = (char *)malloc(sizeof(char) * (len - start) + 1);
	else
		s2 = (char *)malloc(sizeof(char) * (len + 1));
	return (s2);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*s2;

	i = 0;
	if (!s || len == 0)
		return (NULL);
	s2 = ft_malloc(s, start, len);
	if (!s2)
		return (NULL);
	while (i < len && s[i] && start < ft_strlen(s))
	{
		s2[i] = s[start];
		i++;
		start++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*join;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if(!s1)
		return(s2);
	if(!s2)
		return(s1);
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		return (NULL);
	while (s1[i])
	{
		join[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		join[i + j] = s2[j];
		j++;
	}
	join[i + j] = '\0';
	return (join);
}

void	ft_allocate(char **s2, char const *s, char c)
{
	char	**tab_s2;
	char	*tmp;

	tmp = (char *)s;
	tab_s2 = s2;
	while (*tmp)
	{
		while (*s == c)
			++s;
		tmp = (char *)s;
		while (*tmp && *tmp != c)
			++tmp;
		if (*tmp == c || tmp > s)
		{
			*tab_s2 = ft_substr(s, 0, tmp - s);
			s = tmp;
			++tab_s2;
		}
	}
	*tab_s2 = NULL;
}

size_t	count_words(char const *s, char c)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while (s[i] && s)
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i])
				i++;
		}
		else
			i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**s2;
	size_t	size;

	if (!s)
		return (NULL);
	size = count_words(s, c);
	s2 = (char **) malloc (sizeof(char *) * (size + 1));
	if (!s2)
		return (NULL);
	ft_allocate(s2, s, c);
	return (s2);
}


char *alloc_one(char c)
{
    char *new_alloc;

    new_alloc = malloc(sizeof(char) + 1);
    new_alloc[0] = c;
    new_alloc[1] = '\0';
    return(new_alloc);
}

char *join_one(char *str, char c)
{
    char *new_str;
    int i;

    new_str = malloc(sizeof(char) * ft_strlen(str) + 2);
    i = 0;
    while(i < (int)ft_strlen(str))
    {
        new_str[i] = str[i];
        i++;
    }
    new_str[i] = c;
    new_str[i + 1] = '\0';
    return(new_str);
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
	{
		return (0);
	}
	while ((unsigned char)*s1 == (unsigned char)*s2 && *s1 != '\0' && n - 1 > 0)
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*l;
	char	*b;
	size_t	i;
	size_t	j;

	l = (char *)little;
	b = (char *)big;
	if (!*l)
		return ((char *)big);
	i = 0;
	if (len == 0)
		return (NULL);
	while (b[i] && i < len)
	{
		if (b[i] == l[0])
		{
			j = 0;
			while (l[j] && ((i + j) < len) && b[i + j] == l[j])
				j++;
			if (!l[j])
				return ((char *)big + i);
		}
		i++;
	}
	return (0);
}

char	*ft_strdup(char *src)
{
	int		i;
	int		len;
	char	*str;

	len = 0;
	while (src[len])
		len++;
	str = (char*)malloc(sizeof(*str) * (len + 1));
	i = 0;
	while (i < len)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strchr(const char *s, int c)
{
	char	*str;

	str = (char *)s;
	while (*str != c)
	{
		if (*str == '\0')
		{
			return (NULL);
		}
		str++;
	}
	return (str);
}