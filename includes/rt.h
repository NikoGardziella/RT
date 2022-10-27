/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:46:07 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/27 16:05:29 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "libft.h"
# include "SDL.h"
# include "dm_2d.h"
# include "dm_vectors.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <time.h>

# define SCREEN_X 1000
# define SCREEN_Y 600
# define IMG_COUNT 5
# define T_MAX 100000000.0f

# ifndef PI
#  define PI 3.141592
# endif

typedef struct s_env
{
	int test;
}				t_env;

#endif
