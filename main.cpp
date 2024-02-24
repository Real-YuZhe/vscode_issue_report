#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <opencv2/opencv.hpp>

int get_window_size(struct winsize *size) {
  return ioctl(STDOUT_FILENO, TIOCGWINSZ, size);
}

void print_color(uint8_t r1, uint8_t g1, uint8_t b1, u_int8_t r2, uint8_t g2, uint8_t b2) {
  printf("\e[48;2;%d;%d;%dm\e[38;2;%d;%d;%dm▀", r1, g1, b1, r2, g2, b2);
}

void reset_color() {
  printf("\e[0m");
}

int main(int argc, char *argv[]) {
  struct winsize size;
  get_window_size(&size);

  int width = size.ws_col / 2 * 4;
  int height = (size.ws_row - 4) / 2 * 4;

  if (width < 32 || height < 32) {
    fprintf(stderr, "[!] At least 32*32 win size required!\n");
    return -1;
  }

  cv::Mat img = cv::imread(argc == 2 ? argv[1] : "Sample.png");
  double img_factor = (double) img.cols / img.rows;
  double disp_factor = (double) width / height;
  if (img_factor > disp_factor) height  = ((int) (width / img_factor)) / 2 * 2;
  else width = ((int) (height * img_factor)) / 2 * 2;

  printf(" - Image size: [%d * %d]\n", img.cols, img.rows);
  printf(" - Terminal size: [%d * %d]\n", size.ws_col, size.ws_row);
  printf(" - Display size: [%d * %d]\n", width, height);

  cv::Mat dst;
  cv::resize(img, dst, cv::Size(width, height), 0, 0, cv::INTER_LANCZOS4);

  for (int y = 0; y < height; y += 2) {
    putchar('\n');
    for (int x = 0; x < width; x++) {
      int b1 = dst.at<cv::Vec3b>(y, x)[0];
      int g1 = dst.at<cv::Vec3b>(y, x)[1];
      int r1 = dst.at<cv::Vec3b>(y, x)[2];
      int b2 = dst.at<cv::Vec3b>(y + 1, x)[0];
      int g2 = dst.at<cv::Vec3b>(y + 1, x)[1];
      int r2 = dst.at<cv::Vec3b>(y + 1, x)[2];
      print_color(r1, g1, b1, r2, g2, b2);
    }
    reset_color();
  }
  fflush(stdout);
  getchar();

  cv::imshow("Preview", dst);
  while (cv::waitKey(30));
}