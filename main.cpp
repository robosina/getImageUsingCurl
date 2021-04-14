#include <opencv2/opencv.hpp>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>
#include "BSlogger/src/BSlogger.hpp"
using namespace std;
using namespace cv;



extern "C" size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::ostringstream *stream = (std::ostringstream *)userdata;
    size_t             count   = size * nmemb;
    stream->write(ptr, count);
    return(count);
}

Mat grabCurlImage()
{
    std::vector <std::string> images = {
        "https://www.python.org/static/apple-touch-icon-144x144-precomposed.png",
        ""
    };

    CURL     *curl;
    CURLcode res;

    std::ostringstream stream;

    const char *url = images[0].c_str();
    cv::Mat    image;
    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);   // pass the writefunction
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
        res = curl_easy_perform(curl);
        std::string        output   = stream.str();
        std::vector <char> data     = std::vector <char>(output.begin(), output.end()); //convert string into a vector
        cv::Mat            data_mat = cv::Mat(data);                                    // create the cv::Mat datatype from the vector
        image = cv::imdecode(data_mat, 1);                                              //read an image from memory buffer
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
    return(image);
}

void Mat2Byte(cv::Mat InputImage,std::vector<uchar>& output){

    cv::imencode(".jpg",InputImage,output);
}

int main()
{
    LOG_INIT_COUT();
    log.set_log_level(LOG_DEBUG);
    log(LOG_DEBUG)<<"Get Image from URL using curl lib\n";
    cv::Mat image = grabCurlImage();
    if(!image.empty()){
        log(LOG_DEBUG)<<"image has captured image size:"<<image.size()<<"\n";
    }else{
        log(LOG_ERROR)<<"curl not worked -- return\n";
        return 1;
    }

    log(LOG_DEBUG)<<"Trying to Convert Image to std::vector<uchar> -- return\n";
    std::vector<uchar> Bytes;
    Mat2Byte(image,Bytes);
    log(LOG_DEBUG)<<"Bytes.length:"<<Bytes.size()<<"\n";


    log(LOG_DEBUG)<<"Trying to Retrive Image\n";
    cv::Mat result = cv::imdecode(cv::Mat(Bytes),1);

    log(LOG_DEBUG)<<"Size of Retrived image :"<<result.size()<<"\n";

    auto Puttext = [](cv::Mat image,
                      const std::string& text,
                      const cv::Point2f & point){
        cv::putText(image,text,point,cv::FONT_HERSHEY_COMPLEX,0.7,cv::Scalar(0,0,0),5,cv::LINE_AA);
        cv::putText(image,text,point,cv::FONT_HERSHEY_COMPLEX,0.7,cv::Scalar(0,255,0),2,cv::LINE_AA);
    };

    Puttext(image,"before",cv::Point2f(50,50));
    Puttext(result,"after",cv::Point2f(50,50));
    cv::hconcat(image,result,result);
    cv::imshow("image",result);
    cv::waitKey(0);


}
