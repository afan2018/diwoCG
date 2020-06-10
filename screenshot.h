#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <ctime>
#include <cstdio>

//BMP文件头（14字节）
typedef struct                       /**** BMP file header structure ****/
{
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */  
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} MyBITMAPFILEHEADER;

//位图信息头（40字节）
typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} MyBITMAPINFOHEADER;

void save_bmp(const char *filename,unsigned char *rgbbuf,int width,int height)
{
    MyBITMAPFILEHEADER bfh;
    MyBITMAPINFOHEADER bih;
    /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2+sizeof(MyBITMAPFILEHEADER) + sizeof(MyBITMAPINFOHEADER)+width*height*2;
    bfh.bfOffBits = 0x36;

    bih.biSize = sizeof(MyBITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    printf("filename=%s \n", filename);
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        // WHAT TO DO ???
        return;
    }

    /*Write headers*/
    fwrite(&bfType,sizeof(bfType),1,file);
    fwrite(&bfh,sizeof(bfh),1, file);
    fwrite(&bih,sizeof(bih),1, file);

    fwrite(rgbbuf,width*height*3,1,file);
    fclose(file);
}


class screenshot {
private:
    int sx, sy;
    int tx, ty;
    int height;
    void savepic() {
        time_t now = time(NULL);
        char filename[250];
        struct tm* t = gmtime(&now);
        sprintf(filename, "Screenshot_at_%d_%d_%d_%d_%d%d.bmp", t->tm_year+1900,
                t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);//, ctime(&now));
        static unsigned char pic[480 * 480 * 4];
        int lx = std::min(sx, tx), rx = std::max(sx, tx);
        int ly = std::min(sy, ty), ry = std::max(sy, ty);
        int w = rx - lx, h = ry - ly;
        w = std::max(w-1, 0);
        h = std::max(h-1, 0);
        glPixelStorei(GL_PACK_ALIGNMENT, 0);
        glReadPixels(lx, height - ry, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pic);
        save_bmp(filename, pic, w, h);
    }
public:
    screenshot() {
        sx = sy = tx = ty = 0;
    }
    void setheight(int h) {
        height = h;
    }
    void clear() {
        sx = sy = tx = ty = 0;
    }
    void mouse(int button, int state, int x, int y) {
        if (state == GLUT_DOWN) {
            sx = x;
            sy = y;
            tx = x;
            ty = y;
        } else {
            savepic();
            sx = sy = tx = ty = 0;

        }
    }
    void motion(int x, int y) {
        tx = x;
        ty = y;
    }
    void drawRect(int height) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);
        int lx = std::min(sx, tx), rx = std::max(sx, tx);
        int ly = std::min(sy, ty), ry = std::max(sy, ty);
        glVertex2d(lx, height - ly);
        glVertex2d(lx, height - ry);
        glVertex2d(rx, height - ry);
        glVertex2d(rx, height - ly);
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};
