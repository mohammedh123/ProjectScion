#include "Collision.h"


bool Collision::CircleCollision(const sf::Sprite& Object1, const sf::Sprite& Object2)
{
	//gets the bounds with the transformation already acted on the object
	float Radius1 = std::max(Object1.getGlobalBounds().width, Object1.getGlobalBounds().height)/2;
    float Radius2 =  std::max(Object2.getGlobalBounds().width, Object2.getGlobalBounds().height)/2;
	float xd = Object1.getPosition().x - Object2.getPosition().x;
    float yd = Object1.getPosition().y - Object2.getPosition().y;
	
    return CircleCollision(Radius1, sf::Vector2f(Object1.getPosition().x,Object1.getPosition().y), Radius2, sf::Vector2f(Object2.getPosition().x, Object2.getPosition().y));
}
bool Collision::CircleCollision(float radius1, sf::Vector2f position1, float radius2, sf::Vector2f position2)
{
	int dx = position2.x - position1.x;
    int dy = position2.y - position1.y;
    int radii = radius1 + radius2;
    return ((( dx * dx )  + ( dy * dy )) < radii * radii );
}
//Check if the bounding box surrounding the object intersects
bool Collision::BoundingRectCollision(const sf::Sprite& Object1, const sf::Sprite& Object2)
{
	return Object1.getGlobalBounds().intersects(Object2.getGlobalBounds());
}

bool Collision::PixelPerfectCollision(const sf::Sprite& Object1, const sf::Sprite& Object2)
{
	//Since this is expensive, check if the bounding rect intersects
	if(Collision::BoundingRectCollision(Object1, Object2))
	{
		sf::IntRect rect1 = Object1.getTextureRect();
		sf::IntRect rect2 = Object2.getTextureRect();
		sf::Image image1 = Object1.getTexture()->copyToImage();
		sf::Image image2 = Object2.getTexture()->copyToImage();
		sf::Transform transformAToB = Object1.getTransform() * Object2.getInverseTransform();
		sf::Vector2f stepX = transformAToB.transformPoint(sf::Vector2f(1,0));
		sf::Vector2f stepY = transformAToB.transformPoint(sf::Vector2f(0,1));
		sf::Vector2f yPosInB = transformAToB.transformPoint(sf::Vector2f(0,0));

        // For each row of pixels in A
        for (int yA = 0; yA < rect1.height; yA++)
        {
            // Start at the beginning of the row
            sf::Vector2f posInB = yPosInB;

            // For each pixel in this row
            for (int xA = 0; xA < rect1.width; xA++)
            {
                // Round to the nearest pixel
					
                int xB = (int)floor(posInB.x + .5);
                int yB = (int)floor(posInB.y + .5);

                // If the pixel lies within the bounds of B
                if (0 <= xB && xB < rect2.width &&
                    0 <= yB && yB < rect2.height)
                {
						
                    // Get the colors of the overlapping pixels
					if(image1.getPixel(xA, yA).a != 0 && image1.getPixel(xB, yB).a != 0)
					{
                        // then an intersection has been found
                        return true;
                    }
                }

                // Move to the next pixel in the row
                posInB += stepX;
            }

            // Move to the next row
            yPosInB += stepY;
        }
	}
	return false;
}