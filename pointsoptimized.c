#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define NUM_POINTS 1000

#define NUM_LAYERS 3

typedef struct {
	float speed;
	Uint8 brigthness;
	SDL_FPoint *points;
	int count;
} PointLayer;

float randf(void)
{
	return ((float)rand()/(float)(RAND_MAX));
}

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	Uint64 last_time;
	bool running;
	PointLayer layers[NUM_LAYERS];
} Context;

void clean_exit(Context *ctx)
{
	for (int i = 0; i < NUM_LAYERS; i++)
		free(ctx->layers[i].points);
	SDL_DestroyRenderer(ctx->renderer);
	SDL_DestroyWindow(ctx->window);
	SDL_Quit();
}

bool setup(Context *ctx)
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
	int points_per_layer = NUM_POINTS / NUM_LAYERS;

	for (int i = 0; i < NUM_LAYERS; i++)
	{
		ctx->layers[i].count = points_per_layer;
		ctx->layers[i].points = malloc(points_per_layer * sizeof(SDL_FPoint));
		float factor = (float)(i + 1) / NUM_LAYERS;
		ctx->layers[i].speed = 15.0f + (factor * 80.0f);
		ctx->layers[i].brigthness = (Uint8) (50 + (factor * 205));
		for (int j = 0; j < points_per_layer; j++)
		{
			ctx->layers[i].points[j].x = (randf()) * ((float) WINDOW_WIDTH);
			ctx->layers[i].points[j].y = (randf()) * ((float) WINDOW_HEIGHT);
		}
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
void update(Context *ctx)
{
	Uint64 now = SDL_GetTicks64();
	float elapsed = ((float) now - ctx->last_time) / 1000.0f;
	float distance = 0;

	for (int i = 0; i < NUM_LAYERS; i++)
	{
		distance = elapsed * ctx->layers[i].speed;
		for (int j = 0; j < ctx->layers[i].count; j++)
		{
			//ctx->layers[i].points[j].x += distance;
			ctx->layers[i].points[j].y += distance;
			if (ctx->layers[i].points[j].y >= WINDOW_HEIGHT)
			{
				//if (rand() & 1)
				//{
					ctx->layers[i].points[j].x = (randf()) * ((float) WINDOW_WIDTH);
					ctx->layers[i].points[j].y = 0.0f;
				//}
				//else
				//{
				//	ctx->layers[i].points[j].x = 0.0f;
				//	ctx->layers[i].points[j].y = (randf()) * ((float) WINDOW_HEIGHT);
				//}
			}
		}
	}
	ctx->last_time = now;
}

void render(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
	SDL_RenderClear(ctx->renderer);

	for (int i = 0; i < NUM_LAYERS; i++)
	{
		Uint8 b = ctx->layers[i].brigthness;
		SDL_SetRenderDrawColor(ctx->renderer, b, b, b, 255);
		SDL_RenderDrawPointsF(ctx->renderer, ctx->layers[i].points, ctx->layers[i].count);
	}

	// Update renderer
	SDL_RenderPresent(ctx->renderer);
}

int main(void)
{
	Context ctx = {0};

	if (!setup(&ctx))
		return (1);

	while (ctx.running)
	{
		// 1. Handle input
		process_input(&ctx);

		// 2. Update
		update(&ctx);

		// 3. Render
		render(&ctx);
	}
	clean_exit(&ctx);
	return (0);
}