#include <iostream>
#include <math.h>
#include "light.h"
#define M_PI 3.14159265358979323846

Light::Light()
{
    m_intensity=0;
    m_radius=0;
    m_quality=0;
    m_actif=true;
}
 
Light::Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color)
{
    m_position=position;
    m_intensity=intensity;
    m_radius=radius;
    m_color=color;
    m_quality=quality;
}
 
Light::~Light()
{
    m_shape.clear();
}
 
 
void Light::Draw(sf::RenderTarget *App)
{
    // On boucle sur m_shape pour afficher tous les triangles.
    for(int i=0;i<(int)m_shape.size();i++)
		App->draw(m_shape[i]);
}
 
sf::Vector2f Intersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2)
{
    sf::Vector2f i;
 
    if((p2.x - p1.x) == 0 && (q2.x - q1.x) == 0)
        i.x = 0, i.y = 0;
    else if((p2.x - p1.x) == 0)
    {
        i.x = p1.x;
 
        float c = (q2.y - q1.y) / (q2.x - q1.x);
        float d = q1.y - q1.x * c;
 
        i.y = c * i.x + d;
    }
 
    else if((q2.x - q1.x) == 0)
    {
        i.x = q1.x;
 
        float a = (p2.y - p1.y) / (p2.x - p1.x);
        float b = p1.y - p1.x * a;
 
        i.y = a * i.x + b;
    }
    else
    {
        float a = (p2.y - p1.y) / (p2.x - p1.x);
        float b = p1.y - p1.x * a;
 
        float c = (q2.y - q1.y) / (q2.x - q1.x);
        float d = q1.y - q1.x * c;
 
        i.x = (d-b)/(a-c);
        i.y = a * i.x + b;
    }
 
    return i;
}
 
sf::Vector2f Collision(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2)
{
    sf::Vector2f i;
    i = Intersect(p1, p2, q1, q2);
 
    if(((i.x >= p1.x - 0.1 && i.x <= p2.x + 0.1)
        || (i.x >= p2.x - 0.1 && i.x <= p1.x + 0.1))
    && ((i.x >= q1.x - 0.1 && i.x <= q2.x + 0.1)
        || (i.x >= q2.x - 0.1 && i.x <= q1.x + 0.1))
    && ((i.y >= p1.y - 0.1 && i.y <= p2.y + 0.1)
        || (i.y >= p2.y - 0.1 && i.y <= p1.y + 0.1))
    && ((i.y >= q1.y - 0.1 && i.y <= q2.y + 0.1)
        || (i.y >= q2.y - 0.1 && i.y <= q1.y + 0.1)))
        return i;
    else
        return sf::Vector2f (0,0);
 
}
 
void Light::AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, int minimum_wall,std::vector <Wall> &m_wall)
{
    int w = minimum_wall;
    // On boucle sur tous les murs
    for(std::vector<Wall>::iterator IterWall=m_wall.begin()+minimum_wall;IterWall!=m_wall.end();++IterWall,++w)
    {
        // l1 et l2 sont les positions relatives au centre de la lumi�re des deux extr�mit�s du mur
        sf::Vector2f l1(IterWall->pt1.x-m_position.x, IterWall->pt1.y-m_position.y);
        sf::Vector2f l2(IterWall->pt2.x-m_position.x, IterWall->pt2.y-m_position.y);
 
        if(l1.x * l1.x + l1.y * l1.y < m_radius * m_radius)
        {
            sf::Vector2f i = Intersect(pt1,pt2,sf::Vector2f (0,0),l1);
 
            if((pt1.x > i.x && pt2.x < i.x) || (pt1.x < i.x && pt2.x > i.x))
            if((pt1.y > i.y && pt2.y < i.y) || (pt1.y < i.y && pt2.y > i.y))
                if(l1.y > 0 && i.y > 0 || l1.y < 0 && i.y < 0)
                if(l1.x > 0 && i.x > 0 || l1.x < 0 && i.x < 0)
                AddTriangle(i, pt2, w, m_wall), pt2 = i;
        }
        if(l2.x * l2.x + l2.y * l2.y < m_radius * m_radius)
        {
            sf::Vector2f i = Intersect(pt1,pt2,sf::Vector2f (0,0),l2);
 
            if((pt1.x > i.x && pt2.x < i.x) || (pt1.x < i.x && pt2.x > i.x))
            if((pt1.y > i.y && pt2.y < i.y) || (pt1.y < i.y && pt2.y > i.y))
                if(l2.y > 0 && i.y > 0 || l2.y < 0 && i.y < 0)
                if(l2.x > 0 && i.x > 0 || l2.x < 0 && i.x < 0)
                AddTriangle(pt1, i, w, m_wall), pt1 = i;
        }
 
        sf::Vector2f m = Collision(l1, l2, sf::Vector2f(0,0), pt1);
        sf::Vector2f n = Collision(l1, l2, sf::Vector2f(0,0), pt2);
        sf::Vector2f o = Collision(l1, l2, pt1, pt2);
 
        if((m.x != 0 || m.y != 0) && (n.x != 0 || n.y != 0))
            pt1 = m, pt2 = n;
        else
        {
            if((m.x != 0 || m.y != 0) && (o.x != 0 || o.y != 0))
                AddTriangle(m ,o , w, m_wall), pt1 = o;
 
            if((n.x != 0 || n.y != 0) && (o.x != 0 || o.y != 0))
                AddTriangle(o ,n , w, m_wall), pt2 = o;
        }
    }
 
    // Variable qui contiendra l'intensit� calcul�e, pour le d�grad�
    float intensity;
 
    // On ajoute un shape
	m_shape.push_back(sf::ConvexShape(3));
 
    // On lui donne comme point de d�part (0,0), le centre de la lumi�re, avec la couleur et intensit� maximal
	m_shape.back().setPoint(0, sf::Vector2f(0, 0));
 
    // On calcul ou l'on se trouve par rapport au centre, pour savoir � quel intensit� on est
    intensity=m_intensity-sqrt(pt1.x*pt1.x + pt1.y*pt1.y)*m_intensity/m_radius;
    // Et on ajoute un  point au shape
 //   m_shape.back().AddPoint(pt1.x, pt1.y,  sf::Color((int)(intensity*m_color.r/255),
 //                                                    (int)(intensity*m_color.g/255),
 //                                                    (int)(intensity*m_color.b/255)),sf::Color(255,255,255));
 //
 //   // Idem
 //   intensity=m_intensity-sqrt(pt2.x*pt2.x + pt2.y*pt2.y)*m_intensity/m_radius;
 //   m_shape.back().AddPoint(pt2.x, pt2.y,  sf::Color((int)(intensity*m_color.r/255),
 //                                                    (int)(intensity*m_color.g/255),
 //                                                    (int)(intensity*m_color.b/255)),sf::Color(255,255,255));
 //
 //   // On met que le shape soit en Add et on lui donne sa position
 //   m_shape.back().SetBlendMode(sf::Blend::Add);
	m_shape.back().setPoint(1, pt1);
	m_shape.back().setPoint(2, pt2);
	m_shape.back().setFillColor(sf::Color((int)(m_intensity*m_color.r/255),
                                             (int)(m_intensity*m_color.g/255),
                                             (int)(m_intensity*m_color.b/255)));
	m_shape.back().setPosition(m_position);
}
 
void Light::Generate(std::vector <Wall> &m_wall)
{
    // On vide la m�moire
    m_shape.clear();
 
    // buf est l'angle de chaque triangle, c'est donc 2pi divis� par le nombre de triangles
    float buf=(M_PI*2)/(float)m_quality;
 
    // On ajoute tous les triangles qui composent la lumi�re
    for(int i=0;i<m_quality;i++)
    {
        AddTriangle(sf::Vector2f((float)((float)m_radius*cos((float)i*buf))
                                ,(float)((float)m_radius*sin((float)i*buf))) ,
                    sf::Vector2f((float)((float)m_radius*cos((float)(i+1)*buf))
                                ,(float)((float)m_radius*sin((float)(i+1)*buf))),0,m_wall);
    }
}
 
 
// Diff�rentes fonctions pour modifier les attributs de la lumi�re, et pour les r�cup�rer
 
void Light::SetIntensity(float intensity) { m_intensity=intensity; }
void Light::SetRadius(float radius) { m_radius=radius; }
void Light::SetQuality(int quality) { m_quality=quality; }
void Light::SetColor(sf::Color color) { m_color=color; }
void Light::SetPosition(sf::Vector2f position)
{
    m_position=position;
}
 
 
 
float Light::GetIntensity(){ return m_intensity; }
float Light::GetRadius(){ return m_radius; }
int Light::GetQuality(){ return m_quality; }
sf::Vector2f Light::GetPosition(){ return m_position;}
sf::Color Light::GetColor(){ return m_color; }