/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:24:30 by hvecchio          #+#    #+#             */
/*   Updated: 2024/06/26 19:42:43 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_init_threads(t_philo_pack *philo_pack)
{
	int	i;
	pthread_t	end_control;

	i = -1;
	philo_pack->start_time = ft_get_time();
	while (++i < philo_pack->count_philo)
		philo_pack->philos[i].start_time_last_eat = ft_get_time();
	i = -1;
	while (++i < philo_pack->count_philo)
	{
		ft_putstr_fd(" PHILO STARTED \n",2);// TO REMOVE
		if(pthread_create(&(philo_pack->philos[i].thread_ref), NULL, &ft_orchestrate, &(philo_pack->philos[i])))
			return (ft_putstr_fd(" PHILO OVER \n",2), ft_free(philo_pack), ft_error('t'), 0);
	}
	ft_putstr_fd(" Control STARTED \n",2);// TO REMOVE
	if(pthread_create(&end_control, NULL, &ft_end_control, &(philo_pack->philos[i])))
		return (ft_free(philo_pack), ft_error('t'), 0);
	pthread_join(end_control, NULL); //TO REVIEZ IF NEEDED
	return (1);
}

int ft_init_philos(t_philo_pack *philo_pack)
{
	int	i;
	
	i = 0;
	philo_pack->philos = malloc(sizeof(t_philo) * philo_pack->count_philo);
	if (philo_pack->philos == NULL)
		return (ft_error('m'), 0);
	while (i < philo_pack->count_philo)
	{
		philo_pack->philos[i].philo_pack = philo_pack;
		philo_pack->philos[i].id = i + 1;
		philo_pack->philos[i].start_time_last_eat = 0;
		// philo_pack->philos[i].is_eating = 0;
		// philo_pack->philos[i].is_sleeping = 0;
		// philo_pack->philos[i].is_dead = 0;
		philo_pack->philos[i].count_meals = 0;
		if(pthread_mutex_init(&philo_pack->philos[i].started_eating, NULL))
			return (ft_free(philo_pack), ft_error('x'), 0);
		if(pthread_mutex_init(&philo_pack->philos[i].left_fork, NULL))
			return (ft_free(philo_pack), ft_error('x'), 0);
		if (i > 0)
			philo_pack->philos[i - 1].right_fork = philo_pack->philos[i].left_fork;
		if (i + 1 == philo_pack->count_philo && i > 0)
			philo_pack->philos[i].right_fork = philo_pack->philos[0].left_fork;
		i++;
	}
	return (1);
}
