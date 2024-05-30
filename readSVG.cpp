#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void Attribute_transformation(const char* transform_a, const char* origin_a, Point& origin, Point& translation, int& degrees, int&scale){
        //Esta função, tem por objetivo atribuir as transformções a cada objeto
        //inicializa os valores iniciais

        degrees = 0; 

        origin = {0,0};

        scale = 1;

        translation = {0,0};

        // transform_a: Cadeia de caracteres contendo as transformações
        // origin_a: Cadeia de caracteres contendo a origem das transformações
        // origin: Referência ao ponto de origem
        // translation: Referência ao ponto de translação
        // degrees: Referência ao ângulo de rotação
        // scale: Referência ao fator de escala  
        

        if (origin_a) //Se a origem for mencionada, extrai, de espaço branco em espaço branco, as coordenadas dessa origem
        {
            string origin_str = origin_a;
            int x = stoi(origin_str.substr(0, origin_str.find(" "))); //string para inteiros, procura os espaços a começar no índice 0
            int y = stoi(origin_str.substr(origin_str.find(" ") +1));
            origin = {x,y};
        }
        if (transform_a) //Se as transformações forem mencionadas, aplica-as
        {
            string trans_string = transform_a;
            //procura o tipo de translação
            size_t pos_translate = trans_string.find("translate");
            size_t pos_rotation = trans_string.find("rotate");
            size_t pos_scale = trans_string.find("scale");

            if (pos_translate != string::npos){ //se translação
                int tx;
                int ty;
                replace(trans_string.begin(), trans_string.end(), ',', ' ');
                sscanf(trans_string.c_str() + pos_translate, "translate(%d %d)", &tx, &ty);
                translation = {tx, ty};
            }
            if (pos_rotation != string::npos){ //se rotação
                sscanf(trans_string.c_str() + pos_rotation, "rotate(%d)", &degrees);
            }
            if (pos_scale != string::npos){//se scale
                sscanf(trans_string.c_str() + pos_scale, "scale(%d)", &scale);
            }
        }
        
    }
    void convert_elements(XMLElement* xml_elem, vector<SVGElement *>& svg_elements){

            // Itera sobre os elementos filhos do ficheiro base, e verifica o seu tipo, aplicando as suas transformações
            for (XMLElement *elem = xml_elem->FirstChildElement(); elem; elem = elem -> NextSiblingElement()){
                string name = elem -> Name();
                if (name == "ellipse"){ //se o elemento filho for uma elipse
                    // Extrai atributos específicos da elipse

                    int x = elem -> IntAttribute("cx");
                    int y = elem -> IntAttribute("cy");
                    int rx = elem -> IntAttribute("rx");
                    int ry = elem -> IntAttribute("ry");
                    string fill = elem -> Attribute("fill");

                    //Declaração 
                    Point origin;

                    Point translation;

                    int scale;

                    int degrees;
                    

                    const char* transform_a = elem -> Attribute("transform");
                    
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //Chama a função das transformações 
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);

                    //Cria uma nova Elipse e aplica as tranformações
                    Ellipse* ellipse = new Ellipse(parse_color(fill), {x, y}, {rx, ry});
                    
                    ellipse->translate(translation);
                    
                    ellipse->rotate(origin, degrees);
                    
                    ellipse->scale(origin, scale);
                    
                    // Adiciona a elipse ao vetor de elementos SVG.

                    svg_elements.push_back(ellipse);
                }
                else if (name == "circle") //Se o elemento filho for um círculo
                {
                    // Extrai atributos específicos do círculo

                    int cx = elem -> IntAttribute("cx");
                    int cy = elem -> IntAttribute("cy");
                    int r = elem -> IntAttribute("r");
                    string fill = elem -> Attribute("fill");

                    //Declaração de Variáveis
                    Point origin;
                    Point translation;
                    
                    int degrees;
                    int scale;
                    
                    const char* transform_a = elem->Attribute("transform");
                    
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //Chama a função das transformações 
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);

                    //Cria um novo círculo
                    Circle* circle = new Circle(parse_color(fill), {cx, cy}, r);
                
                    circle->translate(translation);
                    
                    circle->rotate(origin, degrees);
                    
                    circle->scale(origin, scale);
                    
                    //Adiciona o círculo ao vetor de elementos SVG
                    svg_elements.push_back(circle);

                }
                else if (name == "rect")//Se o elemento filho for um retângulo 
                {
                    //Extrai atributos específicos do retângulo
                    int x = elem -> IntAttribute("x");
                    int y = elem -> IntAttribute("y");
                    int width = elem -> IntAttribute("width");
                    int height = elem -> IntAttribute("height");
                    string fill = elem -> Attribute("fill");

                    //Declaração de variáveis

                    Point origin;
                    Point translation;

                    int scale;
                    int degrees;
                    
                    
                    const char* transform_a = elem->Attribute("transform");
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //Chama a função das transformações 
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);   

                    //Cria um novo rectângulo e aplica as transformações
                    Rect* rect = new Rect(parse_color(fill), {x, y}, width, height);
                    
                    rect->translate(translation);
                    rect->rotate(origin, degrees);
                    rect->scale(origin, scale);
                    //Adiciona o novo rectângulo ao vetor de elementos SVG
                    svg_elements.push_back(rect);

                    
                }
                else if (name == "polygon")//Se o elemento filho for um Polígono
                {
                    // Extrai atributos específicos do Polígono

                    string fill = elem -> Attribute("fill");
                    string points_string = elem -> Attribute("points");
                    
                    char space = ' ';
                    
                    vector<Point> points;
                    
                    stringstream string_stream_var(points_string);
                    
                    string element_;
                    Point point;
                    
                    // Divide a string de pontos em coordenadas individuais

                    while (getline(string_stream_var, element_, space)) {
                        int x = stoi(element_.substr(0, element_.find(",")));
                        int y = stoi(element_.substr(element_.find(",") + 1));
                        point = {x, y};
                        points.push_back(point);
                    }
                    //Declara Variáveis
                    Point origin;
                    Point translation;
                    
                    int degrees;
                    int scale;
                    
                    const char* transform_a = elem->Attribute("transform");
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //Chama a função das transformações 
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);

                    //Cria novo Polígono com as novas transformações
                    Polygon* polygon = new Polygon(parse_color(fill), points);
                    
                    polygon->translate(translation);
                    polygon->rotate(origin, degrees);
                    polygon->scale(origin, scale);
                    
                    svg_elements.push_back(polygon); //Adiciona o novo Polígono ao svg_elements

                }
                else if (name == "line") //Se o elemento filho for uma linha
                {
                    //Extrai atributos específico da linha
                    int x1 = elem -> IntAttribute("x1");
                    int y1 = elem -> IntAttribute("y1");
                    int x2 = elem -> IntAttribute("x2");
                    int y2 = elem -> IntAttribute("y2");
                    string stroke = elem -> Attribute("stroke");
                    
                    //Declaração de Variáveis

                    Point origin;
                    Point translation;
                    
                    int degrees;
                    int scale;
                    
                    const char* transform_a = elem->Attribute("transform");
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //Chama a função das transformações 
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);

                    
                    // Cria um novo polígono e aplica as transformações
                    Line* line = new Line(parse_color(stroke), {x1, y1}, {x2, y2});
                    
                    line->translate(translation);
                    
                    line->rotate(origin, degrees);
                    
                    line->scale(origin, scale);
                    
                    // Adiciona o polígono ao vetor de elementos SVG
                    svg_elements.push_back(line);

                }
                else if (name == "polyline") //Se o elemento filho for Polilinha
                {
                    // Extrai atributos específicos do Polyline

                    string s = elem -> Attribute("points");
                    char delimiter = ' ';
                    vector<Point> points;
                    stringstream ss(s); //Reparte em palavras numa nova variável chamada de ss
                    string token;
                    Point p;

                    // Divide a string de pontos em coordenadas individuais

                    while (getline(ss, token, delimiter)) {
                        int x = stoi(token.substr(0, token.find(",")));
                        int y = stoi(token.substr(token.find(",") + 1));
                        p = {x, y};
                        points.push_back(p);
                    }
                    //string da cor do traçado
                    string stroke = elem -> Attribute("stroke");
                    
                    //Declara Variáveis
                    Point origin;

                    Point translation;

                    int degrees;

                    int scale;
                    
                    const char* transform_a = elem->Attribute("transform");
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //Chama a função das transformações 
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);

                    // Cria um novo Polyline e aplica as transformações

                    Polyline* polyline = new Polyline(parse_color(stroke), points);
                    
                    //Para "este" novo Polyline
                    polyline->translate(translation);
                    
                    polyline->rotate(origin, degrees);
                    
                    polyline->scale(origin, scale);
                    
                    // Adiciona o polígono ao vetor de elementos SVG
                    svg_elements.push_back(polyline);
                }
                else if (name=="g") //se aparecer algum group
                {
                    vector<SVGElement *>elements;
                    convert_elements(elem, elements);   //Chama a função que aplica as transformações e converte

                    Group* group = new Group(elements); //cria um novo group
                    
                    //Declaração de variáveis
                    Point origin;

                    Point translation;

                    int scale;

                    int degrees;
                    

                    const char* transform_a = elem -> Attribute("transform");
                    
                    const char* origin_a = elem -> Attribute("transform-origin");
                    
                    //chama a função que lê os atributos
                    Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale); 



                    //Aplica transformações
                    group->translate(translation);
                    
                    group->rotate(origin, degrees);
                    
                    group->scale(origin, scale);
                    
                    //adiciona o grupo aoo svg_elements
                    svg_elements.push_back(group);

                }
                else if (name == "use") {
                    // Extrair o valor do href
                    const char* href = elem->Attribute("href");

                    if (href) {
                        // Remover o "#" do valor atribuido ao href
                        string hrefValue(href);
                        hrefValue.erase(remove(hrefValue.begin(), hrefValue.end(), '#'), hrefValue.end());

                        // Procurar a referncia do elemento pelo seu ID
                        XMLElement* referencedElement = nullptr;
                        for (XMLElement* e = xml_elem->FirstChildElement(); e; e = e->NextSiblingElement()) {
                            const char* id = e->Attribute("id");
                            if (id && hrefValue == id) {
                                referencedElement = e;
                                break;
                            }
                        }

                        if (referencedElement) {
                            // Converter a referencia do elemento
                            vector<SVGElement*> referencedSVGElements;
                            convert_elements(referencedElement, referencedSVGElements);

                            // Aplicar as tranformações
                            Point origin;
                            Point translation;
                            int scale;
                            int degrees;

                            const char* transform_a = elem->Attribute("transform");
                            const char* origin_a = elem->Attribute("transform-origin");

                            Attribute_transformation(transform_a, origin_a, origin, translation, degrees, scale);

                            for (auto element : referencedSVGElements) {
                                element->translate(translation);
                                element->rotate(origin, degrees);
                                element->scale(origin, scale);

                                // Adicionar o elemnto transferido ao vetor do svg_elements 
                                svg_elements.push_back(element);
                            }
                        } else {
                            cerr << "Referenced element with id " << hrefValue << " not found." << endl;
                        }
                    }
                }
            }
}
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        // Função para ler um ficheiro SVG e extrair os seus elementos
        // svg_file: Caminho para o ficheiro SVG
        // dimensions: Referência para as dimensões da imagem SVG
        // svg_elements: Vetor para armazenar os elementos SVG extraídos
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        convert_elements(xml_elem, svg_elements); //chama a função para converter novos elementos/grupos
    }
}
