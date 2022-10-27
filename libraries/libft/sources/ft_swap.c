/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 11:33:01 by dmalesev          #+#    #+#             */
/*   Updated: 2022/06/14 11:34:17 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_swap(int *nbr1, int *nbr2)
{
	int	temp;

	temp = *nbr1;
	*nbr1 = *nbr2;
	*nbr2 = temp;
}
