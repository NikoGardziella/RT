/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/28 16:50:54 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	close_prog(void *param, char *exit_msg, int exit_code)
{
	static t_env	*env;

	if (env == NULL)
	{
		env = param;
		return ;
	}
	free_images(env->img, IMAGES);
	//ft_lstdel(&env->scene, &del_object);
	ft_putendl(exit_msg);
	exit (exit_code);
}

void	sdl_init(t_sdl *sdl)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_Quit();
	SDL_CreateWindowAndRenderer(SCREEN_X, SCREEN_Y, 0, &sdl->window, &sdl->renderer);
	if (sdl->window == NULL)
		close_prog(NULL, "Creating window failed...", -1);
	if (sdl->renderer == NULL)
		close_prog(NULL, "Creating renderer failed...", -1);
}

void	process_image(SDL_Renderer *renderer, t_img *img, int mode, void *param)
{
	SDL_Rect	rect[2];

	rect[0] = (SDL_Rect){0, 0, img->dim.size.x, img->dim.size.y};
	rect[1] = (SDL_Rect){img->dim.start.x, img->dim.start.y, img->dim.size.x, img->dim.size.y};
	SDL_LockTexture(img->txtr, NULL, &img->addr, &img->line_length);
	fill_image(img, 0x000000);
	img->draw_func(img, param);
	SDL_UnlockTexture(img->txtr);
	/*TEST if RenderClear faster*/
	SDL_RenderCopy(renderer, img->txtr, &rect[0], &rect[1]);
	if (mode == 1)
		SDL_RenderPresent(renderer);
}


int	main(int argc, char **argv)
{
	t_env	env;

	close_prog(&env, "Initializing close program function.", 42);
	ft_bzero(&env, sizeof(t_env));
	sdl_init(&env.sdl);
	env.img = create_images(env.sdl.renderer, IMAGES);
	if (env.img == NULL)
		close_prog(NULL, "Creating images failed...", -1);
	process_image(env.sdl.renderer, &env.img[0], 1, &env);
	process_image(env.sdl.renderer, &env.img[1], 1, &env);
	while (1)
	{
		if (env.sdl.event.type == SDL_QUIT || env.sdl.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			SDL_Quit();
			break ;
		}
		/*else if (env.sdl.event.type == SDL_WINDOWEVENT)
		{
			if (env.sdl.event.window.event == SDL_WINDOWEVENT_EXPOSED)
				put_images_to_screen(env);
		}*/
		/*Find out what this does!*/
		SDL_WaitEvent(&env.sdl.event);
	}
	SDL_DestroyRenderer(env.sdl.renderer);
	SDL_DestroyWindow(env.sdl.window);
	(void)argc;
	(void)argv;
	return(0);
}
