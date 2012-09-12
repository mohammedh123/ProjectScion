#include "Light.h"
 
#ifndef LIGHTMANAGERH
#define LIGHTMANAGERH
 
#include "Singleton.h"
 
 
class Light_Manager : public CSingleton<Light_Manager>
{
    protected :
 
    Light_Manager();
    ~Light_Manager();
 
     // Les tableaux de murs, lumières statiques et dynamiques
    std::vector <Light> m_StaticLight;
    std::vector <Light> m_DynamicLight;
 
    public :
    // Constructeur et destructeur
    friend Light_Manager* CSingleton<Light_Manager>::GetInstance();
    friend void CSingleton<Light_Manager>::Kill();
 
 
    //Différents moyen d'ajouter une lumière dynamique, soit on l'ajoute sans aucune valeur par défaut, soit on lui donne une lumière par défaut, soit on lui donne ses valeurs "à la main"
    Light_Entity Add_Dynamic_Light();
    Light_Entity Add_Dynamic_Light(Light);
    Light_Entity Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
 
    //Différents moyen d'ajouter une lumière dynamique, soit on lui donne une lumière par défaut, soit on lui donne ses valeurs "à la main"
    //On ne peut ajouter une lumière static sans rien, ça ne servirait à rien car elle ne peut être modifiée par la suite
    Light_Entity Add_Static_Light(Light);
    Light_Entity Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
 
    // Ajouter un mur
    Wall_Entity Add_Wall(sf::Vector2f pt1,sf::Vector2f pt2);
 
    // Désactiver une lumière ou supprimer un mur
    void Delete_Light(Light_Entity);
    void Delete_Wall(Wall_Entity);
 
    void Delete_All_Wall();
    void Delete_All_Light();
 
    // Calculer toutes les lumières dynamiques
    void Generate(const sf::View& view);
    void Generate(Light_Entity);
 
    // Afficher toutes les lumières à l'écran
    void Draw(sf::RenderWindow *App);
 
    // Différentes méthodes pour modifier les attributs d'une lumière, ou les récupérer. Il faut à chaque fois envoyer une Light_Entity en paramètre pour
    // savoir de quelle lumière on parle/
 
    void SetPosition(Light_Entity, sf::Vector2f );
    void SetQuality(Light_Entity, int );
    void SetRadius(Light_Entity, int );
    void SetColor(Light_Entity, sf::Color );
    void SetIntensity(Light_Entity, int);
 
    float GetIntensity(Light_Entity);
    float GetRadius(Light_Entity);
    int GetQuality(Light_Entity);
    sf::Color GetColor(Light_Entity);
    sf::Vector2f GetPosition(Light_Entity);
 
    void SetPosition(Wall_Entity, sf::Vector2f );
 
    sf::Color m_basicLight;
    int m_lightSmooth;
    std::vector <Wall> m_wall;
 
    private:
 
    sf::Shader BlurEffect;
    std::vector<Light>::iterator Iter;
    sf::RenderTexture m_renderImg;
 
};
#endif