#ifndef PROYECTO_1_ROOTMULLER_H
#define PROYECTO_1_ROOTMULLER_H

#include "boost/math/tools/polynomial.hpp"
#include "DeflateMethod.hpp"
#include <complex>


using namespace boost::math::tools;

/**
 * Encuentra las raices de un polinomio, por el metodo de muller.
 * @tparam T
 * @param poly Polinomio para encontrar la raiz.
 * @param xr Aproximacion de raiz inicial.
 * @param h Factor para calcular "x0" y "x1" iniciales.
 * @param eps
 * @param maxit Maximo de iteraciones.
 * @return Raiz encontrada.
 */
template<typename T>
std::complex<T> muller(const polynomial<std::complex<T>> &poly, std::complex<T> xr, std::complex<T> h = 0.0001,
                       T eps = sqrt(std::numeric_limits<T>::epsilon()), int maxit = 100) {
    /*
     * Declaracion de constantes.
     * Error si se hace directo.
     */
    const std::complex<T> CUATRO = 4.0 + 0.0i;
    const std::complex<T> DOS = 2.0 + 0.0i;
    const std::complex<T> CERO = 0.0 + 0.0i;
    const std::complex<T> UNO_PUNTO_CINCO = 1.5 + 0.0i;
    const std::complex<T> UNO = 1.0 + 0.0i;

    std::complex<T> x2, x1, x0;
    std::complex<T> h0, h1, d0, d1, a, b, c;
    std::complex<T> rad, den, dxr;

    x2 = xr;
    x1 = xr == CERO ? UNO_PUNTO_CINCO : xr + h * xr;
    x0 = xr == CERO ? -UNO : xr - h * xr;

    for (int i = 0; i < maxit; ++i) {

        h0 = x1 + x0;
        h1 = x2 - x1;
        d0 = (poly.evaluate(x1) - poly.evaluate(x0)) / h0;
        d1 = (poly.evaluate(x2) - poly.evaluate(x1)) / h1;
        a = (d1 - d0) / (h1 + h0);
        b = a * h1 + d1;
        c = poly.evaluate(x2);

        //con xr negativo esto da nan

        rad = std::sqrt(b * b - CUATRO * a * c);
        if (std::abs(b + rad) > std::abs(b - rad))
            den = b + rad;
        else
            den = b - rad;

        dxr = -DOS * c / den;
        xr = x2 + dxr;

        if (std::abs(dxr) < std::abs(eps * xr)) break;

        x0 = x1;
        x1 = x2;
        x2 = xr;
    }
}

//Calculo de todas las raices del polinomio
template<typename T>
std::vector<T> MullerFull(const polynomial<T> &poly, T xr, bool pulido) {
    polynomial<T> temp(poly);
    polynomial<std::complex<T>> tempcomplex(poly);
    polynomial<std::complex<T>> r;
    std::vector<T> raices;
    T raiz;
    std::complex<T> raiztemp;
    T resi = 0;
    for (int i = 0; i < poly.degree(); i++) {
        raiztemp = muller<T>(temp, xr);
        // Pulido de raices
        if (pulido) {
            // Se llama el metodo con la raiz calculada y el polinomio original
            raiztemp = muller<T>(poly, raiztemp);
        }
        // Se realiza la deflacion del polinomio a evaluar
        if (raiztemp.imag() == 0) {
            raices.push_back(raiz);
            temp = deflate(temp, raiz, resi);
            tempcomplex = polynomial<std::complex<T>>(temp);
            //Si la raiz es imaginaria se muestra el mensaje
        } else {
            std::cout<<"Se encontro una raiz imaginaria:"<<raiztemp.real()<<"+"<<raiztemp.imag()<<"i" << std::endl;
        }
    }
    return raices;
}

#endif //PROYECTO_1_ROOTMULLER_H
