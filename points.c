#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define NUM_POINTS 100000
#define MIN_PIXELS_PER_SECOND 30
#define MAX_PIXELS_PER_SECOND 60

float randf(void)
{
	return ((float)rand()/(float)(RAND_MAX));
}

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	Uint64 last_time;
	bool running;
} Context;

void clean_exit(Context *ctx)
{
	SDL_DestroyRenderer(ctx->renderer);
	SDL_DestroyWindow(ctx->window);
	SDL_Quit();
}

bool setup(Context *ctx, SDL_FPoint *points, float *points_speeds)
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Error al inicializar SDL: %s\n", SDL_GetError());
		return (false);
	}
	ctx->window = SDL_CreateWindow(
		"Moving Points",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
	);
	if (!ctx->window)
	{
		printf("Error al crear la ventana: %s\n", SDL_GetError());
		return (false);
	}
	Uint32 flags = SDL_RENDERER_ACCELERATED;
	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, flags);
	if (!ctx->renderer)
	{
		printf("Error al crear el renderer\n");
		return (false);
	}
	ctx->running = true;

	srand(100);
	for (int i = 0; i < NUM_POINTS; i++)
	{
		points[i].x = (randf()) * ((float) WINDOW_WIDTH);
		points[i].y = (randf()) * ((float) WINDOW_HEIGHT);
		points_speeds[i] = MIN_PIXELS_PER_SECOND +
						((randf()) * 
						(MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
	}
	ctx->last_time = SDL_GetTicks64();
	return (true);
}

void process_input(Context *ctx)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				ctx->running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					ctx->running = false;
				break;
		}
	}
}
void update(Context *ctx, SDL_FPoint *points, float *points_speeds)
{
	Uint64 now = SDL_GetTicks64();
	float elapsed = ((float) now - ctx->last_time) / 1000.0f;
	float distance = 0;

	for (int i = 0; i < NUM_POINTS; i++)
	{
		distance = elapsed * points_speeds[i];
		//points[i].x += distance;
		points[i].y += distance;
		if (points[i].x >= WINDOW_WIDTH || points[i].y >= WINDOW_HEIGHT)
		{
			//if (rand() & 0x1)
			{
				points[i].x = (randf()) * ((float) WINDOW_WIDTH);
				points[i].y = 0.0f;
			}
			//else
			//{
			//	points[i].x = 0.0f;
			//	points[i].y = (randf()) * ((float) WINDOW_HEIGHT);
			//}
			points_speeds[i] = MIN_PIXELS_PER_SECOND +
						((randf()) * 
						(MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
		}
	}

	ctx->last_time = now;
}

void render(Context *ctx, SDL_FPoint *points, float *points_speeds)
{
	SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
	SDL_RenderClear(ctx->renderer);

	float speed_factor = 0;
	Uint8 brightness = 0;
	for (int i = 0; i < NUM_POINTS; i++)
	{
		speed_factor = (points_speeds[i] - MIN_PIXELS_PER_SECOND) / 
						(MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND);

		brightness = (Uint8)(50 + (speed_factor * 205));
		SDL_SetRenderDrawColor(ctx->renderer, brightness, brightness, brightness, 255);
		SDL_RenderDrawPointF(ctx->renderer, points[i].x, points[i].y);
	}

	// Update renderer
	SDL_RenderPresent(ctx->renderer);
}

int main(void)
{
	Context ctx = {0};
	SDL_FPoint points[NUM_POINTS];
	float points_speeds[NUM_POINTS];
	
	if (!setup(&ctx, points, points_speeds))
		return (1);

	while (ctx.running)
	{
		// 1. Handle input
		process_input(&ctx);

		// 2. Update
		update(&ctx, points, points_speeds);

		// 3. Render
		render(&ctx, points, points_speeds);
	}
	clean_exit(&ctx);
	return (0);
}