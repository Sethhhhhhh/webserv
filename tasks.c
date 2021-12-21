#include "../includes/philo.h"

static void	take_fork(t_p *p)
{
	size_t	fork_id;

	fork_id = (p->id + 1) % p->s->amount;
	pthread_mutex_lock(&(p->s->forks[p->id]));
	msg(p, TYPE_FORK);
	if (p->s->amount == 1)
	{
		s_usleep(p->s->time_to_die);
		pthread_mutex_unlock(&(p->s->forks[p->id]));
		pthread_exit(0);
	}
	pthread_mutex_lock(&(p->s->forks[fork_id]));
	msg(p, TYPE_FORK);
}

static void	eat(t_p *p)
{
	msg(p, TYPE_EAT);
	p->is_eating = 1;
	p->last_eat_time = get_time();
	(p->count_eat)++;
	pthread_mutex_lock(&(p->s->lock_m));
	if (p->s->eat_amount > 0 && p->count_eat >= p->s->eat_amount)
	{
		pthread_mutex_unlock(&(p->s->forks[p->id]));
		pthread_mutex_unlock(&(p->s->forks[(p->id + 1) % p->s->amount]));
		p->s->must_eat++;
		p->must_eat = 1;
		if (p->s->must_eat >= p->s->amount)
		{
			p->s->someone_died = 1;
			pthread_mutex_lock(&(p->s->write_m));
			s_putstr_fd(get_msg_type(TYPE_OVER), 1);
			pthread_mutex_unlock(&(p->s->write_m));
		}
		pthread_mutex_unlock(&(p->s->lock_m));
		pthread_exit(0);
	}
	pthread_mutex_unlock(&(p->s->lock_m));
	s_usleep(p->s->time_to_eat);
}

static void	put_fork(t_p *p)
{
	pthread_mutex_unlock(&(p->s->forks[p->id]));
	pthread_mutex_unlock(&(p->s->forks[(p->id + 1) % p->s->amount]));
	p->is_eating = 0;
	msg(p, TYPE_SLEEP);
	s_usleep(p->s->time_to_sleep);
}

void	*tasks(void *p_s)
{
	t_p				*p;

	p = (t_p *)p_s;
	if (p->id % 2)
		s_usleep(p->s->time_to_eat);
	while (1)
	{
		take_fork(p);
		eat(p);
		put_fork(p);
		msg(p, TYPE_THINK);
		if (p->s->someone_died)
			pthread_exit(0);
	}
	pthread_exit(0);
}
