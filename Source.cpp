#include <string>
#include "XLibrary11.hpp"
using namespace std;
using namespace DirectX;
using namespace XLibrary11;

enum Mode
{
    Title, Game
};

Float3 GetRandomPosition()
{
    float range = rand() / (float)RAND_MAX * App::GetWindowSize().x + 500.0f;
    float radian = rand() / (float)RAND_MAX * XM_PI * 2.0f;

    return Float3(cosf(radian) * range, sinf(radian) * range, 0.0f);
}

bool IsHit(Float3 position1, Float3 position2, float range)
{
    if (position1.x - range < position2.x &&
        position1.x + range > position2.x &&
        position1.y - range < position2.y &&
        position1.y + range > position2.y)
    {
        return true;
    }

    return false;
}

int MAIN()
{
    Mode mode = Title;

    Camera camera;
    camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);

    const float playerSpeed = 3.0f;

    Sprite player(L"player.png");
    player.scale = 2.0f;

    const int enemyNum = 300;

    Sprite enemy(L"enemy.png");
    enemy.scale = 2.0f;

    Float3 enemyPosition[enemyNum];

    const int bulletNum = 5;
    const float bulletSpeed = 10.0f;
    int bulletIndex = 0;
    int bulletInterval = 0;

    Sprite bullet(L"bullet.png");
    bullet.scale = 2.0f;

    Float3 bulletPosition[bulletNum];
    float bulletRadian[bulletNum];

    Text titleText(L"シューティング", 32.0f);
    titleText.scale = 2.0f;
    titleText.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);

    int score = 0;

    Text scoreText(L"0", 10.0f);
    scoreText.scale = 10.0f;
    scoreText.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);

    while (App::Refresh())
    {
        camera.Update();

        switch (mode)
        {
        case Title:

            if (App::GetKeyDown(VK_LBUTTON))
            {
                player.position = 0.0f;
                score = 0;
                scoreText.Create(L"0", 10.0f);

                for (int i = 0; i < enemyNum; i++)
                {
                    enemyPosition[i] = camera.position + GetRandomPosition();
                }

                for (int i = 0; i < bulletNum; i++)
                {
                    bulletPosition[i].x = 99999.0f;
                    bulletRadian[i] = 0.0f;
                }

                mode = Game;
            }

            camera.position = 0.0f;

            titleText.Draw();

            scoreText.position = Float3(0.0f, 200.0f, 0.0f);
            scoreText.Draw();

            break;

        case Game:

            if (App::GetKey('D'))
            {
                player.position.x += playerSpeed;
            }
            if (App::GetKey('A'))
            {
                player.position.x -= playerSpeed;
            }
            if (App::GetKey('W'))
            {
                player.position.y += playerSpeed;
            }
            if (App::GetKey('S'))
            {
                player.position.y -= playerSpeed;
            }
            float playerRadian = atan2f(App::GetMousePosition().y, App::GetMousePosition().x);
            player.angles.z = playerRadian * 180.0f / XM_PI;
            player.Draw();

            camera.position = player.position;

            bulletInterval++;
            if (bulletInterval > 10)
            {
                bulletInterval = 0;
                bulletPosition[bulletIndex] = player.position;
                bulletRadian[bulletIndex] = playerRadian;

                bulletIndex++;
                if (bulletIndex >= bulletNum)
                {
                    bulletIndex = 0;
                }
            }

            for (int i = 0; i < enemyNum; i++)
            {
                float enemyRadian = atan2f(player.position.y - enemyPosition[i].y, player.position.x - enemyPosition[i].x);
                enemyPosition[i] += Float3(cosf(enemyRadian), sinf(enemyRadian), 0.0f);

                float hitRange = enemy.GetSize().x / 2.0f * enemy.scale.x;
                for (int j = 0; j < bulletNum; j++)
                {
                    if (IsHit(enemyPosition[i], bulletPosition[j], hitRange))
                    {
                        enemyPosition[i] = camera.position + GetRandomPosition();
                        score++;
                        scoreText.Create(to_wstring(score), 10.0f);
                    }
                }

                if (IsHit(enemyPosition[i], player.position, hitRange))
                {
                    mode = Title;
                }

                enemy.position = enemyPosition[i];
                enemy.Draw();
            }

            for (int i = 0; i < bulletNum; i++)
            {
                bulletPosition[i] += Float3(cosf(bulletRadian[i]), sinf(bulletRadian[i]), 0.0f) * bulletSpeed;
                bullet.position = bulletPosition[i];
                bullet.Draw();
            }

            scoreText.position = camera.position + Float3(0.0f, 200.0f, 0.0f);
            scoreText.Draw();

            break;
        }
    }

    return 0;
}
