## 课程设计报告

2019 - 2020 学年春夏学期 《计算机图形学》

张海川 3180105353

章可循 3180104920

范安东 3180103126

### 实现功能

* 具有基本体素（立方体、球、圆柱、圆锥、多面棱柱、多面棱台）的建模表达能力
  * 曲面细分实现
* 具有基本 OBJ 三维网格导入导出功能
* 具有基本材质、纹理的显示和编辑能力
  * 可运行时调整材质、选择纹理
* 具有基本几何变换功能
  * 旋转、平移、缩放
* 基本光照明模型，具有基本的光源编辑能力
  * 可运行时调整光源的位置、开关
* 能对建模后场景进行漫游
  * 具有 Zoom In/Out, Pan, Orbit, Zoom To Fit 等观察功能
* 提供屏幕截取/保存功能
* 能够提供屏幕截取/保存功能
  * 根据数学原理手工实现

### 交互说明

blahblah 翻译一下readme好了

### 实现思路与代码分析

#### 总体框架设计

麻雀虽小，五脏俱全。我们的项目整体上基于一个场景图（scene graph）的架构进行实现，其中，世界中的每一个物体都是一个`node`对象，其核心代码如下：（省略了部分代码，具体请见[`scene_graph.h`](scene_graph.h)）

```c++
class node {
    protected:
            void transform();
            virtual void colorize();

    public:
        // transformation
        GLfloat translate[3]      = { 0.0f, 0.0f, 0.0f };
        GLfloat scale[3]          = { 1.0f, 1.0f, 1.0f };
        GLfloat center[3]         = { 0.0f, 0.0f, 0.0f };
        mat3    rotate_mat        = mat3::identity();

        // material
        std::shared_ptr<material> mtrl;

        // scene
        const std::string type    = "node";
        bool interactive          = true;
        bool visible              = true;

        virtual void update();
        virtual void render();
};
```

简单地说，每个`node`具有其位置、缩放和旋转（使用一个矩阵表示），它们决定了一个`node`的内部坐标系如何变换到世界坐标系；同时，每个`node`提供`update()`方法，负责更新其数据、以及`render()`方法，负责渲染对象本身。

这样做是为了解决图形学（尤其是在这样一个类似于游戏的需求中）里的一些常见问题：

1. 首先，`OpenGL`的API大量使用了状态机，其状态难以追踪，因此需要通过对其API进行封装，由框架维护`OpenGL`状态机的状态，并对外提供某种声明式的接口，从而降低各个模块之间的耦合程度，降低开发者的心智负担。在我们的框架中，矩阵变换以及材质设置分别通过`transform()`和`colorize()`函数进行封装，保证了其实现的正确性、防止对其他元素的渲染产生干扰，同时使得子类的实现变得十分简便。这样的封装也在其他许多地方有所体现，比如将材质抽象为[`material`类](material.h)等。
2. 其次，`OpenGL`作为一款相对底层的图形API，并不能处理所有的高级任务，比如我们项目中用到的“光线追踪”（从观察者的角度发出一束光线，找到这条线上遇到的第一个物体），就必须自己编写代码——而不是调用API——来实现。如果不使用框架、要求元素将位置写入`translate`等公有参数中，则会造成严重的代码耦合和数据冗余问题。这里，我们通过`node`类，定义了一个所有节点的**最小公共接口**，从而有效地解决了这些问题。
3. 除此之外，场景图所提供的抽象，也允许我们进行进一步的优化——比如在我们的项目中，要求每种节点提供其AABB（*Axis-Aligned Bounding Box*，坐标轴对齐包络盒），用于选中对象并显示选中框。实际上，这一抽象还能有更多价值，比如可以构建一棵八叉树（而不是简单地使用`vector`）、从而对视锥体外的对象进行消隐，大大加速场景渲染。当然，由于我们的项目尚未达到此等复杂程度，这样的优化只会成为overkill而已。
4. 最后，场景图提供了一套行之有效的生命周期管理，比如每个`node`应当在其构造函数中进行初始化（此时`glut`已经完成`OpenGL`上下文的初始化），然后在`update()`中进行逻辑更新（此时不保证`OpenGL`状态机的状态），最后在`render()`中进行绘制（此时所有的`update()`均已执行完毕，并且保证对象坐标系变换到视坐标系等）。这也有助于各个模块的实现者做出合理、可靠、高效的实现，减少可能出现的问题。

综上，我们的项目建立在一系列抽象上——节点`node`、场景图`scene_graph`、光源`light`、控制器`control`、摄像机`camera`……它们是其他一切的基础，是各种复杂功能背后那坚实的后盾。

#### 场景漫游

场景漫游首先是键盘控制摄像机移动——这再简单不过了，稍微复杂一点的是转动视角：在按下鼠标时记录当前的鼠标位置，接着根据鼠标的相对位置更改视角，常见的做法是使用[欧拉角](https://en.wikipedia.org/wiki/Euler_angles) ，不过屏幕只有两个维度，而欧拉角有三个，好在人类视角一般不需要`roll`轴，因此只需要处理`yaw`和`pitch`即可。

我们可以看[`control.h`](control.h)中`control`类的代码：

```c++
class orbit_control : public control {
    private:
        float alpha = 0.0f;
        float beta  = 0.0f;
        float x     = 0.0f;
        float y     = 0.0f;
        float z     = 0.0f;

        void move(float angle, float dist);
    public:
        void update() override;
        ray get_ray() override;
};
```

这里的`alpha`和`beta`两个字段就分别对应了`yaw`和`pitch`的角度，`x`、`y`和`z`则是摄像机当前的位置，需要注意在`OpenGL`中进行变换的顺序：

```c++
glRotatef(beta, 1.0f, 0.0f, 0.0f);
glRotatef(alpha, 0.0f, 1.0f, 0.0f);
glTranslatef(-x, -y, -z);
```

这是因为，逻辑上先平移再旋转，旋转则是先`yaw`再`pitch`，同时`OpenGL`中的矩阵操作是右乘（使用列向量），因此顺序需要反过来。

这一串矩阵变换，实际上描述了**从世界坐标系到视点坐标**的变换，如果我们在视点坐标中取一组基（比如三个轴上的单位向量），并用这串矩阵的逆矩阵进行变换，就可以得到它们在世界坐标中的结果。当我们按下`WASD`键进行位移时，我们也可以用这种方法使得摄像机相对当前视角进行移动，得到良好的漫游效果。

最后，我们还实现了一个十分有趣的应用：选中物体，在视角旋转的过程中保证物体的位置（和角度）完全稳定。分解开来，这需要物体在视角改变前后满足两个条件：

1. 物体的中心位置相对视点坐标不变。
2. 物体的旋转矩阵相对视点坐标不变。

事实上，如果采用四元数的描述方法，这两个条件也可以合成为一个条件。通过上述方法我们可以得到新旧两个**世界坐标到视点坐标**的变换矩阵，我们将物体的（世界坐标下的）中心位置以及旋转矩阵乘上旧矩阵、再乘上新矩阵的逆矩阵，就能够达成这一效果。代码如下：

```c++
auto ro = mat3::rotate(g_down_beta, 1.0f, 0.0f, 0.0f) * mat3::rotate(g_down_alpha, 0.0f, 1.0f, 0.0f);
auto rn = mat3::rotate(beta, 1.0f, 0.0f, 0.0f) * mat3::rotate(alpha, 0.0f, 1.0f, 0.0f);
// dp in world
vec3 dpo = {
        down_pos[_x] - down_ray.x0,
        down_pos[_y] - down_ray.y0,
        down_pos[_z] - down_ray.z0
};
vec3 dpn = ~rn * ro * dpo;
// back to node
down_node->translate[_x] = r.x0 + dpn.data[_x];
down_node->translate[_y] = r.y0 + dpn.data[_y];
down_node->translate[_z] = r.z0 + dpn.data[_z];
down_node->rotate_mat = ~rn * ro * down_mat;
```

#### 体素构建

fad

#### 曲面细分

考虑到与课堂内容的衔接，我们在本次项目中主要使用了legacy OpenGL进行编写，然而，legacy OpenGL的固定渲染管线仅在顶点层面处理光照，即[*Gouraud Shading*](https://en.wikipedia.org/wiki/Gouraud_shading) ，效果并不很好，比如下图：

![](images/doc_1.bmp)

图中，方块的表面上明显出现了一条斜线，这是因为整个面只有两个三角形，无法有效处理高光等对于位置非线性的光照结果。

解决方法也很简单：将每个表面分拆成多块，人为提高计算光照的密度，从而制造高得多的精度，如图：

![](images/doc_2.bmp)

图中是将每个表面拆成约一千个表面的结果，可以看出，光照的计算结果已经十分接近逐片元着色（即[*Phong Shading*](https://en.wikipedia.org/wiki/Phong_shading) 的结果了。

事实上，这一细分的工作原理并不复杂，倒是工程实现更具挑战性一些。核心代码如下：

```c++
#define TESSELLATE
#ifdef TESSELLATE

#define GRANULARITY 5

GLenum mode;
GLfloat normal[3]   = {};

class vertex {
public:
    GLfloat position[3] = {};
    GLdouble tex[2]     = {};
};
typedef std::array<float, 3> point;

std::array<vertex, 4> vertices;
int pointer = {};

void tes_glBegin(GLenum _mode) {
    glBegin(GL_TRIANGLES);
    mode = _mode;
    pointer = 0;
}

void tes_glEnd() {/* omitted */}
void tes_glNormal3fv(const GLfloat arr[]) {/* omitted */}
void tes_glNormal3f(GLfloat a, GLfloat b, GLfloat c) {/* omitted */}
void tes_glTexCoord2d(GLdouble u, GLdouble v) {/* omitted */}

void tes_glVertex3fv(const GLfloat arr[]) {
    std::memcpy(vertices[pointer].position, arr, sizeof(GLfloat) * 3);
    ++pointer;
    switch (mode) {
        case GL_TRIANGLES:
            if (pointer == 3) {
                tessellate(0, 1, 2);
                pointer = 0;
            }
            break;
        case GL_QUADS:
            if (pointer == 3) {
                tessellate(0, 1, 2);
            } else if (pointer == 4) {
                tessellate(2, 3, 0);
                pointer = 0;
            }
            break;
        default: break;
    }
}

void tes_glVertex3f(GLfloat x, GLfloat y, GLfloat z) {/* omitted */}
template <typename T, size_t n>
std::array<T, n> interpolate(std::array<T, n> a, std::array<T, n> b, float t) {/* omitted */}
template <typename T, size_t n>
std::array<T, n> sum(const T a[], const T b[], const T c[], point p) {/* omitted */}
void nail(int a, int b, int c, point p) {/* omitted */}

void fractal(int level, int a, int b, int c, point q, point w, point e) {
    if (level != GRANULARITY) {
        point qw = interpolate(q, w, 0.5f);
        point we = interpolate(w, e, 0.5f);
        point eq = interpolate(e, q, 0.5f);
        fractal(level + 1, a, b, c, eq, q, qw);
        fractal(level + 1, a, b, c, qw, w, we);
        fractal(level + 1, a, b, c, we, e, eq);
        fractal(level + 1, a, b, c, qw, we, eq);
    } else {
        nail(a, b, c, q);
        nail(a, b, c, w);
        nail(a, b, c, e);
    }
}

void tessellate(int a, int b, int c) {/* omitted */}

#define glBegin tes_glBegin
#define glEnd tes_glEnd
#define glNormal3fv tes_glNormal3fv
#define glNormal3f tes_glNormal3f
#define glTexCoord2d tes_glTexCoord2d
#define glVertex3fv tes_glVertex3fv
#define glVertex3f tes_glVertex3f
#endif
```

为了最小化对其他代码的影响，我们通过宏定义的方式重载了`glVertex3f()`等各个函数，在调用时缓存下参数，等一个面片（根据`mode`不同可能是`triangle`或`quad`）的参数齐备之后进行细分和插值。

对于插值，我们采用了分形的的技术，即递归地将一个三角形（对于`quad`则先将其拆成两个三角形）拆分成四个小三角形，直到达到规定的细分层数。我们可以通过隐藏中间的三角形来直观地看到细分效果：（密恐警告）

![](images/doc_3.bmp)

这样的做法更大的价值是他可以提供一种自适应的细分效果，即测算每个部分在视角中所占据的大小，根据是否够大决定是否进一步分形——当然，同样地，我们的项目尚未需要实现这种功能。

#### OBJ 网格导入与导出

fad

#### 材质与纹理显示与编辑

fad

#### 几何变换

zkx

####  光照模型

zkx

#### 屏幕截取

zkx

#### Nurbs 建模

zkx

### 分工

张海川：`scene_graph` 维护、glut 生命周期管理、项目框架设计与协调、曲面细分、运动控制

范安东：场景编辑、体素建模、纹理控制、OBJ 文件导入导出、展示视频剪辑

章可循：场景编辑、变换控制、光源控制、截图、Nurbs 建模、展示视频录制与配音

