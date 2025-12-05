#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction    { LEFT, RIGHT }; // for moving with acceleration
enum EntityStatus { ACTIVE, INACTIVE };
enum GameStatus   { ONGOING, FAILED, ACCOMPLISHED };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;
    
    Texture2D mTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;
    
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    int mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;


    // variables that will allow for moving platforms
    bool mIsMovingPlatform = false,
         mMovingRight = true;

    float mMoveSpeed = 0.0f,
          mStartPoint = 0.0f,
          mMoveDistance = 0.0f;


    // track the collided entity
    Entity* mCollidedEntity = nullptr;

    EntityStatus mEntityStatus = ACTIVE;
    GameStatus mGameStatus = ONGOING;

    bool isColliding(Entity *other) const;
    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void resetColliderFlags() 
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }


    // fuel mechanics
    float mFuel = 100.0f;
    // how much fuel is lost per second
    float mFuelRate = 8.0f;

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;
    static constexpr float ACCELERATION_OF_GRAVITY = 15.0f;
    // diagonal drift
    const float drift = 0.99;


    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::vector<int>animationAtlas);
    ~Entity();

    void update(float deltaTime, Entity *collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void activate()   { mEntityStatus  = ACTIVE;   }
    void deactivate() { mEntityStatus  = INACTIVE; }

    bool isActive() { return mEntityStatus == ACTIVE ? true : false; }

    void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }



    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mScale;                 }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    GameStatus  getGameStatus()             const { return mGameStatus;            }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }
    float       getFuel()                  const { return mFuel;                  }
    
    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }

    std::vector<int> getAnimationIndices() const { return mAnimationIndices; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions) 
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions) 
        { mSpriteSheetDimensions = newDimensions;  }
    void setSpeed(int newSpeed)
        { mSpeed  = newSpeed;                      }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setAngle(float newAngle) 
        { mAngle = newAngle;                       }
    void setGameStatus(GameStatus newStatus) 
        { mGameStatus = newStatus;                 }    

    void setFuel(float newFuel) 
        { mFuel = newFuel;                         }
    
    // create a moving platform 
    void makeMovingPlatform(float speed, float distance);
    // update movement for the moving platform
    void updatePlatform(float deltaTime);

    // use fuel
    void useFuel(float amt) { mFuel -= amt; }
    bool hasFuel() const { return mFuel > 0.0f; }
};

#endif // ENTITY_CPP