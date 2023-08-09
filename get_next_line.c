#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libc.h>
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 5
#endif
int	ft_strlen(char *str);

int	ft_strlcat(char *dst, char *src, int dstsize)
{
	int	i;
	int	dlen;
	int	slen;

	slen = ft_strlen(src);
	if (!dst && dstsize == 0)
		return (slen + dstsize);
	dlen = ft_strlen(dst);
	if (dstsize == 0 || dstsize <= dlen)
		return (slen + dstsize);
	i = 0;
	while (src[i] && (i < dstsize - dlen - 1))
	{
		dst[dlen + i] = src[i];
		i++;
	}
	dst[dlen + i] = '\0';
	return (slen + dlen);
}

int	ft_strlcpy(char *dst, char *src, int dstsize)
{
	int	i;
	int	lsrc;

	lsrc = ft_strlen(src);
	i = 0;
	if (dstsize == 0)
		return (lsrc);
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (lsrc);
}

int	ft_strchr(char *find_in, char to_find)
{
	int i = -1;
	while (find_in[++i])
		if (find_in[i] == to_find)
			return (2);
	return (1);
}

int	ft_strlen(char *str)
{
	int	i = -1;
	while (str[++i])
		;
	return (i);
}

char	*ft_strndup(char *to_dup, int size_dup)
{
	char	*duped;
	int		i = -1;

	duped = malloc(size_dup + 1);
	if (!duped)
		return (NULL);
	while (to_dup[++i] && i < size_dup)
		duped[i] = to_dup[i];
	duped[i] = '\0';
	return (duped);
}

void	my_free(void	*to_free)
{
	free(to_free);
	to_free = NULL;
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*string_joined;
	size_t	ls1;
	size_t	ls2;
	size_t	allocat;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	allocat = (ls1 + ls2) + 1;
	string_joined = malloc(allocat);
	if (!string_joined)
		return (NULL);
	ft_strlcpy(string_joined, s1, ls1 + 1);
	ft_strlcat(string_joined, s2, allocat);
	my_free(s1);
	s1 = NULL;
	return (string_joined);
}

char	*read_fd(char *hold, int fd)
{
	int		readl;
	char	*buf;

	readl = 1;
	while (readl >= 0)
	{
		buf = malloc(BUFFER_SIZE + 1);
		readl = read(fd, buf, BUFFER_SIZE);
		if (readl < 0)
			return (my_free(buf), my_free(hold), NULL);
		buf[readl] = '\0';
		hold = ft_strjoin(hold, buf);
		if (ft_strchr(buf, '\n') || ft_strchr(buf, '\0'))
			break ;
		my_free(buf);
	}
	my_free(buf);
	return (hold);
}

int	find_nl(char *hold)
{
	int	i;

	i = -1;
	while (hold[++i])
		if (hold[i] == '\n')
			return i + 1;
	return (i);
}

char	*split_line(char *hold)
{
	char	*line;
	int		l_nl;

	if (!hold[0])
		return (NULL);
	l_nl = find_nl(hold);
	line = ft_strndup(hold, l_nl);
	if (!line)
		return (NULL);
	return (line);
}

char	*rest_line(char	*hold)
{
	char	*rest;
	int		len_nl;
	int		len_end;
	int		size;

	len_nl = find_nl(hold) - 1;
	len_end = ft_strlen(hold);
	if (len_end - len_nl == 0)
		return (printf("here\n"), my_free(hold), NULL);
	else
		size = len_end - len_nl;
	rest = ft_strndup(hold + len_nl + 1, size);
	return (my_free(hold), rest);
}

char *get_next_line(int fd)
{
	char		*line;
	static char	*hold;

	hold = read_fd(hold, fd);
	if (!hold)
		return (NULL);
	line = split_line(hold);
	hold = rest_line(hold);
	return (line);
}

void	leak(void)
{
	system("leaks hell");
}

int	main(void)
{
	int fd;

	// atexit(leak);
	fd = open("get_next_line.c", O_CREAT | O_RDWR);
	char *line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		printf("%s", line);
		my_free(line);
	}
	my_free(line);
	return (0);
}
