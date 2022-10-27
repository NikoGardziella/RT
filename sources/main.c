/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/27 15:19:19 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
/*
int	main(int argc, char **argv)
{
	SDL_Event	event;

	(void)event;
	if (argc > 2)
	{
		ft_putendl("Too many arguments.");
		return (1);
	}
	SDL_Init(SDL_INIT_VIDEO);
	(void)argv;
	return (0);
}
*/
typedef struct s_sdl
{
    SDL_Event        event;
    SDL_Renderer    *renderer;
    SDL_Window        *win;
    SDL_Texture        *buffer;
    void            *data;
    int                pitch;
}    t_sdl;

void    sdl_init(t_sdl *sdl)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, 0, &sdl->win, &sdl->renderer);
    sdl->buffer = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, 480, 480);
}

void    sdl_buffer(t_sdl sdl)
{
    SDL_UnlockTexture(sdl.buffer);
    SDL_RenderCopy(sdl.renderer, sdl.buffer, NULL, NULL);
    SDL_RenderPresent(sdl.renderer);
    SDL_RenderClear(sdl.renderer);
}

int    main(int argc, char **argv)
{
    t_sdl    sdl;
    //t_util    utils;

    // printf("%f %f %f", test.sphere->center.x, test.sphere->center.y, test.sphere->center.z);
    sdl_init(&sdl);
    SDL_LockTexture(sdl.buffer, NULL, &sdl.data, &sdl.pitch);
    // ray_trace(sdl.data, &utils);
    sdl_buffer(sdl);
    while (1)
    {
        if (sdl.event.type == SDL_QUIT
            || sdl.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            SDL_Quit();
            break ;
        }
        else if (sdl.event.type == SDL_WINDOWEVENT)
            if (sdl.event.window.event == SDL_WINDOWEVENT_EXPOSED)
                sdl_buffer(sdl);
        SDL_WaitEvent(&sdl.event);
    }
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.win);
	(void)argc;
	(void)argv;
    return (0);
}
