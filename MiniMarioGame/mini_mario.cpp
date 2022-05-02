
// g++ mini_mario.cpp -o MINI_MARIO -lBox2D -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio



#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
 


using namespace std;

int main()
{                                                                        
	                 
	float ground_x = 0.0f, ground_y = 800.0f, ground_w = 2048.0f, ground_h = 50.0f;
	float body_x = 500.0f , body_y = 500.0f, body_w = 50.0f, angle ;



    
    // Carico la Musica nel gioco
    sf::Music music ;
    sf::Music music_gameover ;
    music.openFromFile("SUPER_MARIO.ogg") ;
    music.setVolume(60) ;                              
    music.play() ;
    music_gameover.openFromFile("GAME_OVER.ogg") ;

     
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "GIOCO");
    sf::Texture GroundTexture ;
    //create graphics for ground body
    sf::RectangleShape groundBodyVis(sf::Vector2f(ground_w,ground_h));
    groundBodyVis.setFillColor(sf::Color::Yellow);
    //GroundTexture.loadFromFile("ground_pattern.jpg");
    groundBodyVis.setPosition(ground_x, ground_y);
   
    //create graphics for dynamic body
    sf::RectangleShape bodyVis(sf::Vector2f(body_w,body_w));
    sf:: Texture ShapeTexture ;
    bodyVis.setFillColor(sf::Color::Red);
    
    bodyVis.setPosition(body_x,body_y);

	b2Vec2 gravity(0.0f, 9.81f);
	b2World world(gravity);
    
          
	//create physics for ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(ground_x, ground_y);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(ground_w/2, ground_h/2); // ground_w/2, ground_h/2

	groundBody->CreateFixture(&groundBox, 0.0f);

	//create physics for dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(body_x, body_y);
	b2Body* body = world.CreateBody(&bodyDef);
    //get coords and angle from box2d engine, set them to objects attributes   
    b2Vec2 position = body->GetPosition();
    // box2d object and sfml object both use center of mass reference
    body_x = position.x; body_y = position.y; 
    angle = fmod( body->GetAngle()*180.0f/M_PI , 360); //rad to deg
    // update visual object
    bodyVis.setPosition(body_x, body_y);
    bodyVis.setRotation(angle);
    
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(body_w/2, body_w/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);  

	
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;



    while (window.isOpen())
    {
       music.setLoop(true) ;

        
        sf::Event event;
        
        while (window.pollEvent(event))
        {
			
            if (event.type == sf::Event::Closed)
                window.close(); 
            
            /*
              SOPRA --> space
              SOTTO -->  Z maiuscola
              DESTRA --> pulsante dx mouse 
              SINISTRA --> pulsante sx mouse
             */
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {     // SINISTRA 
            	body->ApplyLinearImpulse( b2Vec2(-9000, 0), body->GetPosition(),true);
			}
			
			
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {            // SOPRA
		        body->ApplyLinearImpulse( b2Vec2(0, -8000), body->GetPosition(),true); 
		     }
		     
		     
		     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {            // SOTTO
		        body->ApplyLinearImpulse( b2Vec2(0, 8000), body->GetPosition(),true); 
		     }
		     

            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {      // DESTRA
                body->ApplyLinearImpulse( b2Vec2(9000, 0), body->GetPosition(),true);
             }
            
           
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
			
			
			body->ApplyAngularImpulse(4000, true );
			b2Vec2 position = body->GetPosition();
            // box2d object and sfml object both use center of mass reference
            body_x = position.x; body_y = position.y; 
            angle = fmod( body->GetAngle()*180.0f/M_PI , 360); //rad to deg
            // update visual object
            bodyVis.setPosition(body_x, body_y);
            bodyVis.setRotation(angle);

			
			}
       
        }

        world.Step(timeStep, velocityIterations, positionIterations);
	    b2Vec2 position = body->GetPosition();
        
        
	    bodyVis.setPosition(position.x, position.y);
	    
	    
	    if(position.x < 0 || position.x > 1024 ) {  
	         music.stop() ;
	         music_gameover.setVolume(90) ;
	         music_gameover.play() ;
	         while(music_gameover.getStatus()==2) ;
	         std::cout <<  " ATTENTO , DEVI RIMANERE NELLA SAFE ZONE !  " << endl ;
             return 0;    
         }
 

 
        window.clear();
        window.draw(groundBodyVis);
        window.draw(bodyVis);
        window.display();
    }
    
    std::cout << "Arrivederci ! " << endl ;
    return 0;
}
