#include "SVGElements.hpp"
using namespace std;

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {} //Construtor da classe SVGElement
    SVGElement::~SVGElement() {} //Destrutor da classe SVGElement

    // Ellipse 
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius) 
        //fill: Cor de preenchimento da elipse
        //center: centro da elipse
        //radius: raio da elipse
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
        //usando os 3 elementos, acedendo a PNGImage, utiliza a função draw_ellipse() para desenhar a elipse

    }
    void Ellipse :: translate(const Point &t){
        center = center.translate(t);
        //t: vetor de translação
        // aplica uma translação à elipse
    }
    void Ellipse :: rotate(const Point &origin, int degrees) {
        center = center.rotate(origin, degrees);
        //origin: ponto de origem da rotação
        //degrees: quantos graus pretende rodar a elipse
    }
    void Ellipse :: scale(const Point &origin, int v) {
        center = center.scale(origin, v);
        radius.x *= v;
        radius.y *= v;
        //origin: ponto de origem para aumentar ou diminuir a imagem
        //v: fator de aumento ou diminuição
    }
        SVGElement* Ellipse::elementduplication() const // Função para duplicar elementos
    {
        return new Ellipse(fill, center, radius);  //return a um vetor novo da subclasse grupo
    }
    Circle::Circle(const Color &fill,const Point &center, int radius): fill(fill), center(center), radius(radius)
    {
        //fill: Cor de preenchimento do círculo
        //center: centro do círculo
        //radius: raio do círculo
    }
    void Circle::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, {radius,radius}, fill);
        //usando os 3 elementos(centro, coordenadas do raio e cor), acedendo a PNGImage, utiliza a função draw_ellipse() para desenhar o círculo

    }
     void Circle :: translate(const Point &t){
        center = center.translate(t);
        //t: vetor de translação
    }
    void Circle :: rotate(const Point &origin, int degrees) {
        center = center.rotate(origin, degrees);
        //origin: ponto de origem da rotaçao
        //degrees: ângulo que pretende rodar
    }
    void Circle :: scale(const Point &origin, int v) {
        center = center.scale(origin, v);
        radius *= v;
        //origin: ponto de origem para alterar o tamanho
        //v: fator de alteração do tamanho, alterando o raio

    }
    SVGElement* Circle::elementduplication() const // Função para duplicar elementos
    {
        return new Circle(fill, center, radius);  //return a um vetor novo da subclasse grupo
    }

    Polygon::Polygon(const Color &fill, const std::vector<Point> &points): fill(fill),points(points)
    {
        // fill: Cor de preenchimento do polígono
        // points: Pontos que formam o polígono
    }
    void Polygon::draw(PNGImage &img)const
    {
        
        img.draw_polygon(points, fill);//usando os vertices e a cor, acedendo a PNGImage, utiliza a função draw_polygon() para desenhar o polígono

    }
    void Polygon :: translate(const Point &t){
        for (auto& p : points)
        {
            p = p.translate(t);
        }
        //t: Vetor de translação
        
        
    }
    void Polygon :: rotate(const Point &origin, int degrees) {
        for (auto& p : points)
        {
            p = p.rotate(origin, degrees);
        }
        // origin: Ponto de origem para a rotação
        // degrees: Ângulo de rotação em graus
    }
    void Polygon :: scale(const Point &origin, int v) {
        for(auto& p : points){
            p = p.scale(origin, v);
        }
        // origin: Ponto de origem 
        // v: Fator de alteração do tamanho
    }
    SVGElement* Polygon::elementduplication() const // Função para duplicar elementos
    {
        return new Polygon(fill, points);  //return a um vetor novo da subclasse grupo
    }

    Rect::Rect(const Color &fill, const Point &topLeft, int width, int height): 
    Polygon(fill, {topLeft, topLeft.translate({width-1, 0}), topLeft.translate({width-1, height-1}), topLeft.translate({0, height-1 })})
    {
        // fill: Cor de preenchimento do retângulo
        // topLeft: Ponto superior esquerdo do retângulo
        // width: Largura do retângulo
        // height: Altura do retângulo
        
    }
    SVGElement* Rect::elementduplication() const // Função para duplicar elementos
    {
    const Polygon* polygon = dynamic_cast<const Polygon*>(this);
    const Color& fill = polygon->getFill();
    const std::vector<Point>& points = polygon->getPoints();
    return new Rect(fill, points[0], points[1].x, points[3].y - points[0].y + 1);
    }







    

    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points): stroke(stroke),points(points)
    {
        // stroke: Cor do traçado do conjunto de linhas entre si ligadas
        // points: Pontos que formam o conjunto de linhas
    }
    void Polyline::draw(PNGImage &img) const
    {
        for (size_t i = 0; i < points.size()-1; i++)
        {
            img.draw_line(points[i], points[i+1], stroke);
            /*iterando sobre o conjunto de pontos, criando uma linha com um certo ponto do vetor,
            e o ponto seguinte a ele no vetor, criando assim sucessivas linhas, que
            juntamente com a cor do traçado, acedendo a PNGImage, utiliza a função draw_line() para desenhar o conjunto de linhas */
        }
           
    }
    void Polyline :: translate(const Point &t){
        for (auto& p : points)
        {
            p = p.translate(t);
        }
        // t: Vetor de translação

        
        
    }
    void Polyline :: rotate(const Point &origin, int degrees) {
        for (auto& p : points)
        {
            p = p.rotate(origin, degrees);
        }
        // origin: Ponto de origem para a rotação
        // degrees: Ângulo de rotação em graus
    }
    void Polyline :: scale(const Point &origin, int v) {
        for(auto& p : points){
            p = p.scale(origin, v);
        }
        // origin: Ponto de origem 
        // v: Fator de alteração do tamanho
    }
    SVGElement* Polyline::elementduplication() const // Função para duplicar elementos
    {
        return new Polyline(stroke, points);  //return a um vetor novo da subclasse grupo
    }
  
    Line::Line(const Color &stroke, const Point &start, const Point &end): Polyline(stroke, {start, end})
    {
        // stroke: Cor do traçado da linha
        // start: Ponto de início da linha
        // end: Ponto de fim da linha
    }
    Group::Group(vector<SVGElement*> elements) //Contrutor do grupo com o vetor que guarda os elementos
        : elements(elements)
    {
    }
    Group::~Group() //Destrutor
    {
        for (auto element: elements){
            delete element;
        }
    }
    void Group::draw(PNGImage &img) const //Desenha os grupos, iterando sobre os elementos e aplicando a função draw() a cada elemento
    {
        for (auto element : elements){
            element -> draw(img);
        }  
    }
    void Group::translate(const Point &t)//Aplica uma translação a todos os elementos, resultando na translação total do conjunto
    {
        for (auto element: elements){
            element -> translate(t);
        }
    }
    void Group::rotate(const Point &origin, int degrees)//Aplica uma rotação a todos os elementos, resultando na rotação total do conjunto
    {
        for (auto element: elements){
            element -> rotate(origin, degrees);
        }
    }
    void Group::scale(const Point &origin, int v)//Altera o tamanho a todos os elementos, resultando na total alteração do tamanho do conjunto
    {
        for (auto element: elements){
            element -> scale(origin, v);
        }
    }
    SVGElement* Group::elementduplication() const // Função para duplicar elementos
    {
        vector < SVGElement* > dpelement;
        for(auto& element: elements){
            dpelement.push_back(element->elementduplication());
        }
        return new Group(dpelement);  //return a um vetor novo da subclasse grupo
    }
}