#ifndef ENTITYDIRECTION_H
#define ENTITYDIRECTION_H

#include "Map.h"
class Entity; 

enum Direction   { LEFT_DIRECTION, UP_DIRECTION, RIGHT_DIRECTION, DOWN_DIRECTION,
                    EAT_RIGHT_DIRECTION, EAT_LEFT_DIRECTION, IDLING_DIRECTION, PET };
enum EntityStatusDirection { ACTIVE_DIRECTION, INACTIVE_DIRECTION                        };
enum EntityTypeDirection   { PLAYER_DIRECTION, NPC_DIRECTION, NONE_DIRECTION             };
enum AITypeDirection       { WANDERER_DIRECTION, FOLLOWER_DIRECTION, FLYER_DIRECTION     };
enum AIStateDirection      { WALKING_DIRECTION, IDLE_DIRECTION, FOLLOWING_DIRECTION      };


class EntityDirection
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
    
    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    bool mIsJumping = false;
    float mJumpingPower = 0.0f;

    // flying path
    float mFlyTime = 0.0f;
    float mBaseY = 0.0f;

    int mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatusDirection mEntityStatus = ACTIVE_DIRECTION;
    EntityTypeDirection   mEntityType;

    AITypeDirection  mAIType;
    AIStateDirection mAIState;

    bool isColliding(EntityDirection *other) const;

    void checkCollisionY(EntityDirection *collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map *map);

    void checkCollisionX(EntityDirection *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Map *map);
    
    void resetColliderFlags() 
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);
    void AIActivate(Entity *target);
    void AIWander();
    void AIFollow(Entity *target);
    void AIFly();

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    //  butterfly bobbing
    static constexpr float FLY_AMPLITUDE =60.0f;
    static constexpr float FLY_FREQUENCY = 0.15f;

    EntityDirection();
    EntityDirection(Vector2 position, Vector2 scale, const char *textureFilepath, 
        EntityTypeDirection entityType);
    EntityDirection(Vector2 position, Vector2 scale, const char *textureFilepath, 
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::map<Direction, std::vector<int>> animationAtlas, 
        EntityTypeDirection entityType);
    ~EntityDirection();

    void update(float deltaTime, Entity *player, Map *map, 
        EntityDirection *collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping = true;  }
    void activate()   { mEntityStatus  = ACTIVE_DIRECTION;   }
    void deactivate() { mEntityStatus  = INACTIVE_DIRECTION; }
    void displayCollider();

    bool isActive() { return mEntityStatus == ACTIVE_DIRECTION ? true : false; }

    void moveUp()
    {
        mMovement.x = 0.0f;
        mMovement.y = -1.0f;
        mDirection  = UP_DIRECTION;
    }
    void moveDown()
    {
        mMovement.x = 0.0f;
        mMovement.y =  1.0f;
        mDirection  = DOWN_DIRECTION;
    }
    void moveLeft()
    {
        mMovement.x = -1.0f;
        mMovement.y =  0.0f;
        mDirection  = LEFT_DIRECTION;
    }
    void moveRight()
    {
        mMovement.x =  1.0f;
        mMovement.y =  0.0f;
        mDirection  = RIGHT_DIRECTION;
    }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;                 }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getJumpingPower()          const { return mJumpingPower;          }
    bool        isJumping()                const { return mIsJumping;             }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }
    EntityTypeDirection  getEntityType()            const { return mEntityType;            }
    AITypeDirection      getAIType()                const { return mAIType;                }
    AIStateDirection     getAIState()               const { return mAIState;               }

    
    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }

    // check for collisions from the lpayer's perspective
    bool enemyCollision(EntityDirection *other) { return isColliding(other); }

    std::map<Direction, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

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
    void setJumpingPower(float newJumpingPower)
        { mJumpingPower = newJumpingPower;         }
    void setAngle(float newAngle) 
        { mAngle = newAngle;                       }
    void setEntityType(EntityTypeDirection entityType)
        { mEntityType = entityType;                }
    void setDirection(Direction newDirection)
    { 
        mDirection = newDirection;

        if (mTextureType == ATLAS) 
        {   
            mAnimationIndices   = mAnimationAtlas.at(mDirection);
            mCurrentFrameIndex  = 0;
            mAnimationTime      = 0.0f;
            if (!mAnimationIndices.empty()) mCurrentFrameIndex %= 
            mAnimationIndices.size();
            

        }
    }
    void setAIState(AIStateDirection newState)
        { mAIState = newState;                     }
    void setAIType(AITypeDirection newType)
        { mAIType = newType;                       }
    void setFlyDirection(Direction newDirection)
        { mDirection = newDirection;               }
    // fly back and forth
    void flyDirection(float left, float right)
        {
            if (mPosition.x < left) setDirection(RIGHT_DIRECTION);
            else if (mPosition.x > right) setDirection(LEFT_DIRECTION);
        }

};

#endif // ENTITY_CPP