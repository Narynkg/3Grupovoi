#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

using namespace cv;
using namespace std;

Mat showHistogram(Mat& img) {
    int bins = 256;
    int nc = img.channels();
    vector<Mat> hist(nc);
    for (int x = 0; x < hist.size(); x++)
        hist[x] = Mat::zeros(1, bins, CV_32SC1);
    for (int x = 0; x < img.rows; x++) {
        for (int j = 0; j < img.cols; j++) {
            for (int k = 0; k < nc; k++) {
                uchar val = nc == 1 ? img.at<uchar>(x,j) : img.at<Vec3b>(x,j)[k];
                hist[k].at<int>(val) += 1;
            }
        }
    }
    int hmax[3] = {0,0,0};
    for (int x = 0; x < nc; x++) {
        for (int j = 0; j < bins-1; j++)
            hmax[x] = hist[x].at<int>(j) > hmax[x] ? hist[x].at<int>(j) : hmax[x];
    }
    const char* wname[3] = { "blue", "green", "red" };
    Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };
    vector<Mat> canvas(nc);
    for (int x = 0; x < nc; x++) {
        canvas[x] = Mat::ones(125, bins, CV_8UC3);
        for (int j = 0, rows = canvas[x].rows; j < bins-1; j++) {
            line(
                    canvas[x],
                    Point(j, rows),
                    Point(j, rows - (hist[x].at<int>(j) * rows/hmax[x])),
                    nc == 1 ? Scalar(200,200,200) : colors[x],
                    1, 8, 0
            );
        }
    }
    Mat v = canvas[0];
    return v;
}
int main() {
    Mat img = cvLoadImage("/home/azamat/a/len.png");
    cvtColor(img, img, CV_RGB2GRAY);
    Mat res = img.clone();
    double normal[256], q[256], u[256], pix[256], s[256], s_[256], q_[256], temp[256];
    int r = 256, p1 = 0, p2 = 0, b = 0, l = 0, con = 0;;
    for (int x = 0; x < 256; x++) normal[x] = 0;
    for (int x = 0; x < img.rows; x++) for (int y = 0; y < img.cols; y++) normal[(img.at<uchar>(x, y))] += 1;
    for (int x = 0; x < 256; x++) {
        normal[x] /= (img.cols * img.rows);
        q[x] = round((255 * normal[x]));
    }
        for (int x = 0; x < 256; x++) {
            if (x <= 28) {
                u[x] = 0.6;
                pix[x] = 28;
            } else if (x > 28 && x <= 56) {
                u[x] = 0.97;
                pix[x] = 56;
            } else if (x > 56 && x <= 85) {
                u[x] = 0.3;
                pix[x] = 86;
            } else if (x > 86 && x <= 112) {
                u[x] = 0.3;
                pix[x] = 112;
            } else if (x > 112 && x <= 140) {
                u[x] = 0.4;
                pix[x] = 140;
            } else if (x > 140 && x <= 168) {
                u[x] = 0.05;
                pix[x] = 168;
            } else if (x > 168 && x <= 196) {
                u[x] = 0.01;
                pix[x] = 196;
            } else if (x > 196 && x <= 224) {
                u[x] = 0.01;
                pix[x] = 224;
            } else if (x > 224 && x <= 255) {
                u[x] = 0.05;
                pix[x] = 255;
            }
    }
    for (int x = 0; x < 256; x++) s[x] = round((255 * u[x]));
    for (int x = 0; x < 256; x++) temp[x] = q[x];
    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < 256; y++) {
            if (temp[y] != -1 && temp[y] == s[x]) {
                for (int z = 0; z <= l; z++) {
                    if (q_[z] == s[x]) {
                    }
                }
                if (con == 0) {
                    s_[l] = temp[y];
                    q_[l] = s[x];
                    l++;
                }
            }
        }
    }
    for (int x = 0; x < res.rows; x++)
        for (int z = 0; z < res.cols; z++)
            for (int y = 0; y < l; y++)
                if (q[res.at<uchar>(x, z)] == s_[y]) res.at<uchar>(x, z) = (uchar) pix[y];
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    namedWindow("Or Histogramm", CV_WINDOW_AUTOSIZE);
    namedWindow("Result", CV_WINDOW_AUTOSIZE);
    namedWindow("Histogramm", CV_WINDOW_AUTOSIZE);
    imshow("Original", img);
    imshow("Or Histogramm", showHistogram(img));
    imshow("Result", res);
    imshow("Histogramm", showHistogram(res));
    waitKey(0);
    return 0;
}