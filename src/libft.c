#include "ft_ls.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = dst;
	s = src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

void addToList(struct dirent *entry, t_output **output) {
    t_output *new_node = malloc(sizeof(t_output));
    if (!new_node) {
        perror("malloc");
        freeList(*output);
        exit(EXIT_FAILURE);
    }
    
    new_node->entry = malloc(sizeof(struct dirent));
    if (!new_node->entry) {
        perror("malloc");
        free(new_node);
        freeList(*output);
        exit(EXIT_FAILURE);
    }

    ft_memcpy(new_node->entry, entry, sizeof(struct dirent));
    new_node->next = NULL;

    if (*output == NULL) {
        *output = new_node;
    } else {
        t_output *current = *output;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}


int	ft_strlowncmp(const char *s1, const char *s2, size_t size)
{
	size_t			i;
	char	*s11;
	char	*s22;

	s11 = ft_strdup(s1);
	s22 = ft_strdup(s2);
	i = 0;
	ft_tolower(s11);
	ft_tolower(s22);
	while ((s11[i] == s22[i] && s11[i] != '\0' && s22[i] != '\0') && i <= size)
	{
		i++;
	}
	int x = s11[i] - s22[i];
	free(s11);
	free(s22);
	return (x);
}

int	ft_strlowcmp(const char *s1, const char *s2)
{
	size_t			i;
	char	*s11;
	char	*s22;

	s11 = ft_strdup(s1);
	s22 = ft_strdup(s2);
	i = 0;
	ft_tolower(s11);
	ft_tolower(s22);
	while ((s11[i] == s22[i] && s11[i] != '\0' && s22[i] != '\0'))
	{
		i++;
	}
	int x = s11[i] - s22[i];
	free(s11);
	free(s22);
	return (x);
}


void freeMat(char **mat, int count)
{
	int i = 0;

	while(i < count)
	{
		free (mat[i]);
		i++;
	}
	free (mat);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*s11;
	unsigned char	*s22;

	s11 = (unsigned char *)s1;
	s22 = (unsigned char *)s2;
	i = 0;
	while ((s11[i] == s22[i] && s11[i] != '\0' && s22[i] != '\0'))
	{
		i++;
	}
	return (s11[i] - s22[i]);
}

int compare_int(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

int compare_paths(const void *a, const void *b) {
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;
    return ft_strcmp(path1, path2); // capire
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*s11;
	unsigned char	*s22;

	s11 = (unsigned char *)s1;
	s22 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while ((s11[i] == s22[i] && s11[i] != '\0') && ((i + 1) < n))
	{
		i++;
	}
	return (s11[i] - s22[i]);
}


void printList(t_output *head) {
	t_output *current = head;
	while (current != NULL) {
		ft_printf("PRINTLIST -> %s\n", current->entry->d_name);
		current = current->next;
	}
}

void error(DIR *dir)
{
	if (dir != NULL)
		closedir(dir);
	perror("Bad malloc\n");
	exit (1);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	int		len;
	char	*dest;

	i = 0;
	len = ft_strlen(s1);
	dest = malloc((len + 1) * sizeof(char));
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
void flagset(t_flag *flags)
{
	flags->a = false;
	flags->l = false;
	flags->R = false;
	flags->r = false;
	flags->t = false;
}

void	ft_tolower(char *str)
{
	for (int i = 0; i  < ft_strlen(str); i++)
	if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + ('a' - 'A');
}