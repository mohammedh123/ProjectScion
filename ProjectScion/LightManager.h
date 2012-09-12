#include "Light.h"
 
#ifndef LIGHTMANAGERH
#define LIGHTMANAGERH
 
#include "Singleton.h"
 
 
class Light_Manager : public CSingleton<Light_Manager>
{
    protected :
 
    Light_Manager();
    ~Light_Manager();
 
     // Les tableaux de murs, lumi�res statiques et dynamiques
    std::vector <Light> m_StaticLight;
    std::vector <Light> m_DynamicLight;
 
    public :
    // Constructeur et destructeur
    friend Light_Manager* CSingleton<Light_Manager>::GetInstance();
    friend void CSingleton<Light_Manager>::Kill();
 
 
    //Diff�rents moyen d'ajouter une lumi�re dynamique, soit on l'ajoute sans aucune valeur par d�faut, soit on lui donne une lumi�re par d�faut, soit on lui donne ses valeurs "� la main"
    Light_Entity Add_Dynamic_Light();
    Light_Entity Add_Dynamic_Light(Light);
    Light_Entity Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
 
    //Diff�rents moyen d'ajouter une lumi�re dynamique, soit on lui donne une lumi�re par d�faut, soit on lui donne ses valeurs "� la main"
    //On ne peut ajouter une lumi�re static sans rien, �a ne servirait � rien car elle ne peut �tre modifi�e par la suite
    Light_Entity Add_Static_Light(Light);
    Light_Entity Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
 
    // Ajouter un mur
    Wall_Entity Add_Wall(sf::Vector2f pt1,sf::Vector2f pt2);
 
    // D�sactiver une lumi�re ou supprimer un mur
    void Delete_Light(Light_Entity);
    void Delete_Wall(Wall_Entity);
 
    void Delete_All_Wall();
    void Delete_All_Light();
 
    // Calculer toutes les lumi�res dynamiques
    void Generate(const sf::View& view);
    void Generate(Light_Entity);
 
    // Afficher toutes les lumi�res � l'�cran
    void Draw(sf::RenderWindow *App);
 
    // Diff�rentes m�thodes pour modifier les attributs d'une lumi�re, ou les r�cup�rer. Il faut � chaque fois envoyer une Light_Entity en param�tre pour
    // savoir de quelle lumi�re on parle/
 
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