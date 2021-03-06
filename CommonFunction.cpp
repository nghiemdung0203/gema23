#include "CommonFunction.h"

CommonFuntion::CommonFuntion()
{
    window = NULL;
    renderer = NULL;
    gravity = 0.05;
    speed = 1;
    speedFly = -1.5;
    blank = 180;
    Score = 0;
    checkAddScore = true;
    check1 = false;
    checkover = false;
    g_music = NULL;
}

bool CommonFuntion::getcheck()
{
    return check1;
}


bool CommonFuntion::Running()
{
    return running;
}

void CommonFuntion::Initializer()
{
    SDL_Init(SDL_INIT_EVERYTHING);
        window = SDL_CreateWindow("My game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (window)
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer)
            {
                cout<<"sucessed"<<endl;
                running = true;
                player = texture::Texture("Image/Flappybird.png", renderer);
                background = texture::Texture("Image/BackgroundFlappyBird.png", renderer);
                grass = texture::Texture("Image/base.png", renderer);
                gameover = texture::Texture("Image/gameover.png", renderer);
                mainmenu = texture::Texture("Image/background.png", renderer);
                buttonStart = texture::Texture("Image/startbutton.png", renderer);
                g_music = Mix_LoadWAV("music/flappy_assets_jump.wav");
            }
            else
            {
                cout<<"failed created renderer"<<endl;
            }
        }
        else
        {
            cout<<"window not created"<<endl;
        }
         int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                   cout<<"SDL_image could not initialize! SDL_image Error: %s\n"<<IMG_GetError()<<endl;
                    running = false;
                }
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
                {
                    cout<<Mix_GetError()<<endl;
                }
        playerdest.y = pipes.front().h+100;


        for (int i = 0; i <= WIDTH+100; i += 250)
        {
            PipePair pipePump = generate_pipe_pair();
            pipePump.first.x = i;
            pipePump.second.x = i;
            pipes.push_back(pipePump.first);
            pipes.push_back(pipePump.second);
            for (SDL_Rect p : pipes)
            {
                if ((playerdest.x >= pipePump.first.x+pipePump.first.w) && checkAddScore == true)
                {
                    Score++;
                    checkAddScore = false;
                }
            }
        }
}


void CommonFuntion::setrect()
{

    playersrc.h = 445;
    playersrc.w = 512;
    playersrc.x = playersrc.y = 0;

    playerdest.h = 45;
    playerdest.w = 60;
    playerdest.x = 100;
    speed += gravity;
     if (playerdest.y > 700)
     {
        gravity = 0;
        speed =0;
     }
     else
     {
         playerdest.y += speed + 0.5 * (gravity);
     }

    grasssrc.x = 0;
    grasssrc.y = 0;
    grasssrc.h = 112;
    grasssrc.w = 336;

    grassdest.h = 100;
    grassdest.w = WIDTH;
    grassdest.x = 0;
    grassdest.y = HEIGHT - grassdest.h;

    gameoversrc.x = 0;
    gameoversrc.y = 0;
    gameoversrc.h = 42;
    gameoversrc.w = 192;

    gameoverdest.w = 200;
    gameoverdest.h = 150;
    gameoverdest.x = (WIDTH/2)-100;
    gameoverdest.y = (HEIGHT/2)+100;
}

void CommonFuntion::setrectMenu()
{
     buttonsrc.x = buttonsrc.y = 0;
     buttonsrc.h = 311;
     buttonsrc.w = 860;

    buttondest.x = (WIDTH/2)-100;
    buttondest.y = (HEIGHT/2)-50;
    buttondest.h =100;
    buttondest.w = 150;

}


void CommonFuntion::handleEvent()
{
     while (SDL_PollEvent(&event1))
     {
		switch (event1.type)
		{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event1.key.keysym.sym)
				{
					case SDLK_SPACE:
						if (playerdest.y + playerdest.h > 0)
							speed = speedFly;
							Mix_PlayChannel(-1, g_music, 0);
						break;
				}
		}
     }
}

void CommonFuntion::menu()
{
        setrectMenu();
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, mainmenu, NULL, NULL);
        SDL_RenderCopy(renderer, buttonStart, &buttonsrc, &buttondest);
        SDL_RenderPresent(renderer);
}

void CommonFuntion::changeState()
{
    SDL_PollEvent(&event1);
    if (event1.type == SDL_QUIT)
    {
        running = false;
    }
    SDL_GetMouseState(&x,&y);
    if((x>=buttondest.x)&&(x<=buttondest.x+buttondest.w)&&(y>=buttondest.y)&&(y<=buttondest.y+buttondest.h))
       {
            SDL_SetTextureColorMod(mainmenu,64, 64, 64) ;
            if(event1.type==SDL_MOUSEBUTTONDOWN)
    {
      check1=true;
    }
    }
    else
    SDL_SetTextureColorMod(mainmenu,250,0,0) ;
}



SDL_Texture* texture::Texture(const char* filelocation, SDL_Renderer* ren)
{
    SDL_Surface* surface;
    surface = IMG_Load(filelocation);
    SDL_Texture* tex;
    tex = SDL_CreateTextureFromSurface(ren, surface);
    return tex;
}

void CommonFuntion::game_Over()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameover, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void CommonFuntion::Render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_RenderCopy(renderer, player, &playersrc, &playerdest);
    SDL_RenderCopy(renderer, grass, &grasssrc, &grassdest);
     handle_pipes();
     draw_pipes();
    for (deque<SDL_Rect>::iterator it = pipes.begin(); it != pipes.end(); it+=1)
    {
		it->x -= 1;
    SDL_RenderPresent(renderer);
    }

     SDL_RenderPresent(renderer);
}

void CommonFuntion::clearUP()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(mainmenu);
    SDL_DestroyTexture(buttonStart);
    SDL_DestroyTexture(gameover);
    Mix_FreeChunk(g_music);
}



void CommonFuntion::Game()
{
    frameStart = SDL_GetTicks();
        if ((frameStart - lastFrame) < (1000/60))
        {
            SDL_Delay((1000/60) - (frameStart - lastFrame));
        }
        lastFrame = frameStart;
        setrect();
        Render();
        handleEvent();
}

PipePair CommonFuntion::generate_pipe_pair()
{
    PipePair pipePump;

    pipePump.first.h = rand() % 160 + 140;
    pipePump.second.y = pipePump.first.h + blank;

    pipePump.first.x = WIDTH - pipePump.first.w;
    pipePump.first.y = 0;
    pipePump.first.w = 35;

    pipePump.second.x = WIDTH - pipePump.first.w;
	pipePump.second.w = 35;
	pipePump.second.h = HEIGHT - pipePump.second.y;

	return pipePump;

}

void CommonFuntion::handle_pipes()
{
    if (pipes.front().x + pipes.front().w > 0)
		return ;
	pipes.pop_front();
	pipes.pop_front();
	PipePair pipe_pair = generate_pipe_pair();
	pipes.push_back(pipe_pair.first);
	pipes.push_back(pipe_pair.second);
}


void CommonFuntion::draw_pipes()
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 85, 255);
	for (SDL_Rect p : pipes)
		SDL_RenderFillRect(renderer, &p);
}


bool CommonFuntion::game_over()
{
	for (SDL_Rect p : pipes)
		if (SDL_HasIntersection(&p, &playerdest) == SDL_TRUE)
		{
			return true;
			checkover = true;
		}
	if (playerdest.y + playerdest.w >= HEIGHT)
		return true;
		checkover = true;
	return false;
}

void CommonFuntion::run()
{
    menu();
    changeState();
    while(check1 == true)
    {
        Game();
        if (checkover = true)
        {
            while(game_over() == true)
            {
                game_Over();
            }
        }
    }
    cout<<Score<<endl;

}
