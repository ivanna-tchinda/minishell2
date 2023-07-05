#include "minishell.h"

int	ft_lenbuff(char *buffer)
{
	size_t	len;

	len = 0;
	if (!buffer)
		return (0);
	while (buffer[len] && buffer[len] != '\n')
		len++;
	if (buffer[len] == '\n')
		len++;
	return (len);
}

int	is_endline(char *varstat)
{
	int	i;

	if (!varstat)
		return (0);
	i = 0;
	while (varstat[i])
	{
		if (varstat[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*fill_beforeend(char *buffer)
{
	size_t	lenbuffer;
	size_t	i;
	char	*line;

	lenbuffer = ft_lenbuff(buffer);
	i = 0;
	if (!*buffer)
		return (NULL);
	line = malloc(sizeof(char) * (lenbuffer + 1));
	if (!line)
		return (NULL);
	else
	{
		while (i < lenbuffer)
		{
			line[i] = buffer[i];
			i++;
		}
		line[i] = '\0';
		return (line);
	}
}

char	*fill_afterend(char *buffer)
{
	size_t		i;
	size_t		lenbuffer;
	char		*end;

	i = 0;
	lenbuffer = ft_lenbuff(buffer);
	if (!buffer[lenbuffer])
	{
		free(buffer);
		return (NULL);
	}
	end = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - lenbuffer + 1));
	if (!end)
		return (NULL);
	while (buffer[lenbuffer])
		end[i++] = buffer[lenbuffer++];
	end[i] = '\0';
	free(buffer);
	return (end);
}

char	*ft_read(int fd, char *varstat)
{
	char	*buffer;
	int		num;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	num = 1;
	while (!is_endline(varstat) && num != 0)
	{
		num = read(fd, buffer, BUFFER_SIZE);
		if ((int)num == -1)
		{	
			free(buffer);
			return (NULL);
		}
		buffer[num] = '\0';
		if (!varstat)
			varstat = ft_strdup(buffer);
		else
			varstat = ft_strjoin(varstat, buffer);
	}
	free(buffer);
	return (varstat);
}

char	*get_next_line(int fd)
{
	static char	*varstat;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	varstat = ft_read(fd, varstat);
	if (!varstat)
		return (NULL);
	line = fill_beforeend(varstat);
	varstat = fill_afterend(varstat);
	return (line);
}