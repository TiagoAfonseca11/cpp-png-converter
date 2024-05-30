//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

using namespace std;


namespace svg
{
    class SVGElement    //Classe base para os elementos SVG
    {
    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0; //Função para desenhar o elemento em PNGImage
        virtual void translate(const Point &t) = 0; //Função para aplicar a translação à função
        virtual void rotate(const Point &origin, int degrees) = 0; //Função para aplicar a translação à função
        virtual void scale(const Point &origin, int v) = 0; //Função para alterar tamanho do elemento
        virtual SVGElement* elementduplication()const = 0; // Função para duplicar elementos

    };
    
    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);     // Função para ler um ficheiro SVG e extrair as dimensões e os elementos SVG

    void convert(const std::string &svg_file,
                 const std::string &png_file); //Converte SVG para PNG

    class Ellipse : public SVGElement //Subclasse para representar uma Elipse
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override; //Desenha a Elipse em PNGImage
        void translate(const Point &t) override; // Translação da Elipse
        void rotate(const Point &origin, int degrees) override; // Roda a Elipse 
        void scale(const Point &origin, int v) override; //Altera um certo número de vezes o tamanho da Elipse
        SVGElement* elementduplication() const override; // Função para duplicar elementos

    private:
        Color fill; //Cor de preenchimento da elipse
        Point center; //Centro da elipse
        Point radius; //Raio da elipse
    };

    class Circle : public SVGElement //Subclasse para representar um Círculo
    {
    public:
        Circle(const Color &fill, const Point &center, int radius);
        void draw(PNGImage &img) const override; //Desenha o Círculo em PNGImage
        void translate(const Point &t) override; //Aplica uma translação ao círculo
        void rotate(const Point &origin, int degrees) override; //Roda o Círculo
        void scale(const Point &origin, int v) override; //Altera um certo número de vezes o tamanho do Círculo
        SVGElement* elementduplication() const override; // Função para duplicar elementos

    private:
        Color fill; //Cor de preenchimento do círculo
        Point center; //Centro do círculo
        int radius; //Raio do círculo
    };
    class Polygon : public SVGElement //Subclasse para representar um Polígono
    {
    public:
        Polygon(const Color &fill, const std::vector<Point> &points);
        void draw(PNGImage &img) const override; // Desenha o polígono em PNGImage
        void translate(const Point &t) override;//Aplica uma translação ao Polígono
        void rotate(const Point &origin, int degrees) override; //Aplica uma rotação ao Polígono
        void scale(const Point &origin, int v) override; //Altera o tamanho do polígono, tendo em conta o inteiro v (altera v vezes o tamanho)
        SVGElement* elementduplication() const override; // Função para duplicar elementos
    const std::vector<Point>& getPoints() const {
        return points;
    }
        const Color& getFill() const {
        return fill;
    }
    private:
        Color fill; //Cor de preenchimento do Polígono
        std::vector<Point> points; //Vetor que contém o conjunto de pontos necessários para formar o Polígono
    };

    class Rect : public Polygon //Subclasse para representar um retângulo
    {
    public:
        Rect(const Color &fill, const Point &topLeft, int width, int height);
        SVGElement* elementduplication() const override; // Função para duplicar elementos
    };

 
    class Polyline : public SVGElement //Subclasse para representar um conjunto de linhas entre si ligadas
    {
    public:
        Polyline(const Color &stroke, const std::vector<Point> &points);
        void draw(PNGImage &img) const override; //Desenha o conjunto de linhas
        void translate(const Point &t) override; //Aplica uma translação ao conjunto
        void rotate(const Point &origin, int degrees) override; //Aplica uma rotação ao conjunto
        void scale(const Point &origin, int v) override; //Altera o tamanho do conjunto
        SVGElement* elementduplication() const override; // Função para duplicar elementos
    private:
        Color stroke; //Cor do traçado do elemento
        std::vector<Point> points; //Conjunto de pontos que formam o elemento
    };
    class Line : public Polyline //Subclasse para representar uma só linha
    {
    public:
        Line(const Color &stroke, const Point &start, const Point &end); //Linha descrita através de 2 pontos que lhe pertençam e a sua cor
        //SVGElement* elementduplication() const override; // Função para duplicar elementos
    };
    
    class Group : public SVGElement
    {
    public:
        Group(vector<SVGElement*> elements); //Construtor
        virtual ~Group();   //destrutor
        void draw(PNGImage &img) const override; //desenha o grupo
        void translate(const Point &t) override; //aplica uma translação ao grupo
        void rotate(const Point &origin, int degrees) override; //aplica uma rotação ao grupo
        void scale(const Point &origin, int v) override; //altera o tamanho do grupo
        SVGElement* elementduplication() const override; // Função para duplicar elementos
    private:
        vector<SVGElement *> elements; //Vetor com os diversos elementos guardados
    };
}
#endif