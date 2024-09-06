/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_realloc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 00:02:58 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/05 19:50:17 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	*minitalk_memset(void *b, int c, size_t n)
{
	size_t			i;
	unsigned char	*u_b;
	unsigned char	u_c;

	i = 0;
	u_b = (unsigned char *)b;
	u_c = (unsigned char)c;
	while (i < n)
	{
		u_b[i] = u_c;
		i++;
	}
	return (b);
}

void	*minitalk_memcpy(void *s1, const void *s2, size_t n)
{
	size_t				i;
	unsigned char		*dst;
	const unsigned char	*src;

	i = 0;
	dst = (unsigned char *)s1;
	src = (const unsigned char *)s2;
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
	return (dst);
}

void	*minitalk_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size + sizeof(size_t));
	if (ptr == NULL)
		return (NULL);
	minitalk_memset(ptr, 0, size + sizeof(size_t));
	*(size_t *)ptr = size;
	return ((char *)ptr + sizeof(size_t));
}

void	minitalk_meta_free(void *ptr)
{
	void	*real_ptr;

	if (ptr)
	{
		real_ptr = (char *)ptr - sizeof(size_t);
		free(real_ptr);
	}
}

void	*minitalk_realloc(void *ptr, size_t size)
{
	void	*real_ptr;
	size_t	old_size;
	void	*new_ptr;

	if (size == 0)
	{
		minitalk_meta_free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (minitalk_malloc(size));
	real_ptr = (char *)ptr - sizeof(size_t);
	old_size = *(size_t *)real_ptr;
	new_ptr = minitalk_malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	if (old_size < size)
		minitalk_memcpy(new_ptr, ptr, old_size);
	else
		minitalk_memcpy(new_ptr, ptr, size);
	minitalk_meta_free(ptr);
	return (new_ptr);
}
