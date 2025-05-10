#ifndef __UI_HPP__
#define __UI_HPP__

#ifdef NDEBUG
#define DRAW_DEBUG_RECT
#else
#define DRAW_DEBUG_RECT \
    sf::RectangleShape ____(sf::Vector2f(globalSize[Direction::HORIZONTAL], globalSize[Direction::VERTICAL])); \
    ____.setPosition(sf::Vector2f(globalPosition[Direction::HORIZONTAL], (float)globalPosition[Direction::VERTICAL])); \
    ____.setOutlineColor(sf::Color::Yellow); \
    ____.setOutlineThickness(1); \
    ____.setFillColor(sf::Color::Transparent); \
    screen.draw(____)
#endif

#define _ASSETS_ "../assets/"

/**
 * @brief 实现 UI 界面的布局和绘制。
 * @namespace ui
 * 
 * @par REQUIRES
 * NONE
 * 
 * @par PROVIDES
 * 各种组件类。
 */

#include <sstream>
#include <string>
#include <list>
#include <cmath>
#include <functional>
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <cassert>
#include "string_integral.hpp"

namespace ui{

/*****************************************
 * @brief 所有组件类的声明及它们的继承关系。*
 * ********************************************
 */
/**
 * @class 组件类
 * @brief 所有组件的基类。
 * @note 抽象类
 */
class Control;
    /**
     * @class 容器类
     * @brief 所有容器类的基类。
     * @note 抽象类
     */
    class Container;
        /**
         * @class 平凡容器类
         * @brief 仅为组件提供尺寸和位置的参考。
         * @note minSize 属性由用户决定。
         */
        class Flat;
            /**
             * @class 窗口类
             * @brief 所有组件都要放在窗口上才能起作用。
             */
            class Screen;
        /**
         * @class 中心布局类
         * @brief 将组件居中。
         * @note 仅能接受一个子组件。
         * @note minSize 属性由组件决定。
         */
        class Center;
        /**
         * @class 线性布局类
         * @brief 按行或列排列子组件的水平布局类和垂直布局类的基类。
         * @note 抽象类
         */
        class LinearBox;
            /**
             * @class 水平布局类
             * @brief 按行排列子组件。
             * @note minSize 属性由组件决定；但非占比模式下水平方向由用户决定。
             */
            class HorizontalBox;
            /**
             * @class 垂直布局类
             * @brief 按列排列子组件。
             * @note minSize 属性由组件决定；但非占比模式下垂直方向由用户决定。
             */
            class VerticalBox;
        /**
         * @class 滚动线性布局类
         * @brief 能够滚动的线性布局。
         * @note 抽象类
         */
        class ScrollingBox;
            /**
             * @class 水平滚动线性布局类
             * @brief 按行排列子组件，并能滚动。
             * @note minSize 属性，垂直方向由组件决定，水平方向由用户决定。
             */
            class HorizontalScrollingBox;
            /**
             * @class 垂直滚动线性布局类
             * @brief 按列排列子组件，并能滚动。
             * @note minSize 属性，水平方向由组件决定，垂直方向由用户决定。
             */
            class VerticalScrollingBox;
        /**
         * @class 边距容器类
         * @brief 给组件添加边距。
         * @note 仅能接受一个子组件。
         * @note 无视子组件的期望尺寸和位置。
         * @note minSize 属性由组件决定。
         */
        class Margin;
    /**
     * @class 间隔类
     * @brief 用于调整组件之间的距离或绘制一个矩形。
     * @note minSize 属性由用户决定。
     */
    class Spacer;
    /**
     * @class 标签类
     * @brief 显示文本的组件。
     * @note minSize 属性由组件决定。
     */
    class Label;
    /**
     * @class 按钮类
     * @brief 响应用户按键操作的组件。
     * @note minSize 属性由组件决定。
     */
    class Button;
    /**
     * @class 输入框类
     * @brief 允许用户输入文本的组件。
     * @note minSize 属性由组件决定。
     * @note exceedLimitCallback 回调函数 event 参数失效。
     */
    class InputBox;
    /**
     * @class 滚动条类
     * @brief 水平滚动条类和垂直滚动条的基类。
     * @note 抽象类
     * @note 请注意处理非用户造成的 rate 属性的变化。
     */
    class ScrollBar;
        /**
         * @class 水平滚动条类
         * @brief 响应用户滚动操作的组件。
         * @note minSize 属性由用户决定。
         */
        class HorizontalScrollBar;
        /**
         * @class 垂直滚动条类
         * @brief 响应用户滚动操作的组件。
         * @note minSize 属性由用户决定。
         */
        class VerticalScrollBar;
    /**
     * @class 加载指示器类
     * @brief 显示加载交互的组件。
     * @note minSize 属性由用户决定。
     * @note callBack 回调函数 event 参数失效。
     */
    class LoadingRing;
    /**
     * @class 带文本加载指示器类
     * @brief 显示加载交互的组件，并显示文本。
     * @note minSize 属性由组件决定。
     * @note countCallback、finishCallback 回调函数 event 参数失效。
     */
    class LoadingRingWithText;
    /**
     * @class 图片框类
     * @brief 显示图片的组件。
     * @note minSize 属性由用户决定。
     */
    class PictureBox;

/*******************************************
 * @brief 所有组件类的接口声明。*
 * **********************************************
 */
class Control{
public:
    using Callback = std::function<void (const sf::String &, const sf::Event &)>;
    static const Callback DO_NOTHING;
    void SetName(const sf::String &newName) noexcept { name = newName; }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    /**
     * @enum 方向枚举
     */
    enum class Direction{
        HORIZONTAL, // 水平方向
        VERTICAL    // 垂直方向
    };
    /**
     * @enum 尺寸值类型枚举
     */
    enum class ValueType{
        PERCENTAGE, // 百分值
        ABSOLUTE    // 绝对值
    };
    /**
     * @enum 预设尺寸和位置枚举
     */
    enum class Preset{
        PLACR_AT_FRONT,  PLACR_AT_END,  PLACR_AT_CENTER,  // 置于容器前端、末端、中间
        WRAP_AT_FRONT,   WRAP_AT_END,   WRAP_AT_CENTER,   // 紧缩在容器前端、末端、中间
        FILL_FROM_FRONT, FILL_FROM_END, FILL_FROM_CENTER  // 自容器前端、末端、中间填充
    };
    /**
     * @fn 判断屏幕坐标是否在组件内
     * @param x 坐标 x
     * @param y 坐标 y
     * @return 是否在组件内
     */
    bool IsInside(int x, int y) const noexcept;

    /********************************************
     * @brief 期望尺寸和位置的设置接口。          *
     * @note 会引起父容器的“内容尺寸和位置的变化”。*
     * ******************************************
     */
    /**
     * @fn 设置期望尺寸值类型
     * @param direction 方向
     * @param valueType 尺寸值类型
     * @note 百分值表示相对于父容器的尺寸，绝对值表示像素值。
     */
    void SetSizeValueType(Direction direction, ValueType valueType)    noexcept;
    /**
     * @fn 设置自动紧缩
     * @param direction 方向
     * @param flag 是否自动紧缩
     * @note 仅当期望尺寸值类型为绝对值时有效，当值为 true 时，组件的尺寸会自动缩小到最小尺寸，并使期望尺寸失效。
     */
    void SetSizeWrap     (Direction directrion, bool flag)             noexcept;
    /**
     * @fn 设置期望尺寸
     * @param direction 方向
     * @param value 期望尺寸
     * @note 当期望尺寸值类型为百分值时，其值表示所占父容器的百分比或权重；当期望尺寸值类型为绝对值时，要求自动紧缩为 false 时有效，其值为绝对尺寸。
     */
    void SetSize         (Direction directrion, unsigned int value)    noexcept;
    /**
     * @fn 设置最小尺寸
     * @param direction 方向
     * @param absolute 最小绝对尺寸
     * @note 默认该值由用户自由设定，但更多情况是由组件自行决定。此时请注意该值的初始化。
     * @see “组件类一览”
     */
    void SetMinSize      (Direction directrion, unsigned int absolute) noexcept;
    /**
     * @fn 设置组件的中心点
     * @param direction 方向
     * @param percentage 从前端算起中心点所在位置相对于最终尺寸的百分比
     */
    void SetCenter    (Direction directrion, int percentage) noexcept;
    /**
     * @fn 设置组件的锚点
     * @param direction 方向
     * @param percentage 从父容器前端算起锚点所在位置相对于父容器最终尺寸的百分比
     */
    void SetAnchor    (Direction directrion, int percentage) noexcept;
    /**
     * @fn 设置组件的位置
     * @param direction 方向
     * @param absolute 中心点偏离锚点的绝对位置
     */
    void SetPosition  (Direction directrion, int absolute)   noexcept;
    /**
     * @fn 设置组件的可见性
     * @param flag 是否可见
     */
    void SetVisible (bool flag) noexcept;

    /*****************************************
     * @brief 封装后的设置期望尺寸和位置的接口。*
     * ***************************************
     */
    void SetPreset(Direction direction, Preset preset) noexcept;
    void SetHSizeValueType(ValueType valueType)   noexcept { SetSizeValueType(Direction::HORIZONTAL, valueType); }
    void SetVSizeValueType(ValueType valueType)   noexcept { SetSizeValueType(Direction::VERTICAL,   valueType); }
    void SetHSizeWrap     (bool flag)             noexcept { SetSizeWrap(Direction::HORIZONTAL, flag); }
    void SetVSizeWrap     (bool flag)             noexcept { SetSizeWrap(Direction::VERTICAL,   flag); }
    void SetHSize         (unsigned int value)    noexcept { SetSize(Direction::HORIZONTAL, value); }
    void SetVSize         (unsigned int value)    noexcept { SetSize(Direction::VERTICAL,   value); }
    void SetHMinSize      (unsigned int absolute) noexcept { SetMinSize(Direction::HORIZONTAL, absolute); }
    void SetVMinSize      (unsigned int absolute) noexcept { SetMinSize(Direction::VERTICAL,   absolute); }
    void SetHCenter       (int percentage)        noexcept { SetCenter(Direction::HORIZONTAL, percentage); }
    void SetVCenter       (int percentage)        noexcept { SetCenter(Direction::VERTICAL,   percentage); }
    void SetHAnchor       (int percentage)        noexcept { SetAnchor(Direction::HORIZONTAL, percentage); }
    void SetVAnchor       (int percentage)        noexcept { SetAnchor(Direction::VERTICAL,   percentage); }
    void SetHPosition     (int absolute)          noexcept { SetPosition(Direction::HORIZONTAL, absolute); }
    void SetVPosition     (int absolute)          noexcept { SetPosition(Direction::VERTICAL,   absolute); }
    void SetHPreset       (Preset preset)         noexcept { SetPreset(Direction::HORIZONTAL, preset); }
    void SetVPreset       (Preset preset)         noexcept { SetPreset(Direction::VERTICAL,   preset); }
    void SetSizeValueType (ValueType valueType)                            noexcept { SetHSizeValueType(valueType);   SetVSizeValueType(valueType); }
    void SetSizeWrap      (bool flag)                                      noexcept { SetHSizeWrap     (flag);        SetVSizeWrap     (flag); }
    void SetSize          (unsigned int valueH, unsigned int valueV)       noexcept { SetHSize         (valueH);      SetVSize         (valueV); }
    void SetMinSize       (unsigned int absoluteH, unsigned int absoluteV) noexcept { SetHMinSize      (absoluteH);   SetVMinSize      (absoluteV); }
    void SetCenter        (int percentageH, int percentageV)               noexcept { SetHCenter       (percentageH); SetVCenter       (percentageV); }
    void SetAnchor        (int percentageH, int percentageV)               noexcept { SetHAnchor       (percentageH); SetVAnchor       (percentageV); }
    void SetPosition      (int absoluteH, int absoluteV)                   noexcept { SetHPosition     (absoluteH);   SetVPosition     (absoluteV); }
    void SetPreset        (Preset preset)                                  noexcept { SetHPreset       (preset);      SetVPreset       (preset); }
    /**
     * @fn 将组件添加到容器中
     * @param container 容器
     */
    void AddTo            (Container *container)                           noexcept;

    /************************************************
     * @brief 要求派生提供的抽象方法，以实现具体的功能。*
     * **********************************************
     */
    /**
     * @fn 更新组件内容尺寸位置
     * @param resetMinSize 是否重置最小尺寸
     * @note 调用时机：组件最终尺寸和位置变化时，调用 UpdateInQueue(false)；组件内容尺寸和位置变化时，调用 UpdateInQueue(true)。
     * @note 实现功能：确定内容的最终尺寸和位置。
     */
    virtual void Update  (bool resetMinSize = true)                                 noexcept = 0;
    void UpdateInQueue(bool argv = true) noexcept { inQueue = true; resetMinSize = resetMinSize || argv; } // 用以提升性能
    /**
     * @fn 处理事件
     * @param event 事件
     * @note 调用时机：父容器转送事件时。
     * @note 实现功能：处理事件。
     */
    virtual void Process (const sf::Event &event, const sf::RenderWindow &screen)   noexcept = 0;
    /**
     * @fn 绘制组件
     * @param screen 待绘制的窗口
     * @note 调用时机：父容器需要组件被绘制时。
     * @note 实现功能：绘制组件。
     */
    virtual void Draw    (sf::RenderWindow &screen)                                 noexcept = 0;
    /**
     * @fn 帧刷新组件
     * @note 调用时机：每一帧刷新时。
     * @note 实现功能：刷新组件。
     */
    virtual void Tick    ()                                                         noexcept;

    /*****************************************
     * @brief 父容器更新内容的尺寸和位置的接口。*
     * ***************************************
     */
    /**
     * @fn 设置父容器
     * @param container 父容器
     */
    void SetParent        (Container *container)                        noexcept;
    /**
     * @fn 设置最终尺寸
     * @param direction 方向
     * @param absolute 最终绝对尺寸
     */
    void SetGlobalSize    (Direction directrion, unsigned int absolute) noexcept;
    /**
     * @fn 设置最终位置
     * @param direction 方向
     * @param absolute 最终绝对位置
     */
    void SetGlobalPosition(Direction directrion, int absolute)          noexcept;
    /**
     * @note 外部调用时，要先调用 screen.Tick()。
     */
    unsigned int GetGlobalSize    (Direction direction) const noexcept { return globalSize[direction]; }
    /**
     * @note 外部调用时，要先调用 screen.Tick()。
     */
    int          GetGlobalPosition(Direction direction) const noexcept { return globalPosition[direction]; }

    ValueType    GetSizeValueType(Direction direction) const noexcept { return sizeValueType[direction]; }
    bool         GetSizeWrap     (Direction direction) const noexcept { return sizeWrap[direction]; }
    unsigned int GetSize         (Direction direction) const noexcept { return size[direction]; }
    unsigned int GetMinSize      (Direction direction) const noexcept { return minSize[direction]; }
    int  GetCenter  (Direction direction) const noexcept { return center[direction]; }
    int  GetAnchor  (Direction direction) const noexcept { return anchor[direction]; }
    int  GetPosition(Direction direction) const noexcept { return position[direction]; }
    bool GetVisible() const noexcept { return visible; }

    virtual ~Control() noexcept;
protected:
    sf::String name = "default";
    bool inQueue = false;
    bool resetMinSize = false; // 用以提升性能

    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    static Direction GetAnotherDirection(Direction direction) noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    template <typename DataType>
    class Around{
    public: 
        DataType horizontal; 
        DataType vertical; 
        DataType       &operator[](Direction direction)       noexcept 
        {
            switch (direction) {
                case Direction::HORIZONTAL: return horizontal; break;
                case Direction::VERTICAL:   return vertical;   break;
                default:
                    assert(false); // Unknown direction.
                    break; 
            }
            return horizontal;
        }
        const DataType &operator[](Direction direction) const noexcept 
        {
            switch (direction) {
                case Direction::HORIZONTAL: return horizontal; break;
                case Direction::VERTICAL:   return vertical;   break;
                default:
                    assert(false); // Unknown direction.
                    break; 
            }
            return horizontal;
        }
    };

    /*****************************************
     * @brief 期望尺寸和位置属性。*
     * **************************
     */
    Around<ValueType>    sizeValueType = {ValueType::ABSOLUTE, ValueType::ABSOLUTE};
    Around<bool>         sizeWrap      = {false, false};
    Around<unsigned int> size          = {200, 100};
    Around<unsigned int> minSize       = {0, 0};
    Around<int> center   = {0, 0};
    Around<int> anchor   = {0, 0};
    Around<int> position = {0, 0};
    bool visible = true;

    /*****************************************
     * @brief 最终尺寸和位置属性。*
     * **************************
     */
    Container           *parent         = nullptr;
    Around<unsigned int> globalSize     = {200, 100};
    Around<int>          globalPosition = {0, 0};
};

class Container : public Control{
public:
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    using Children = std::vector<Control *>;

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    /**
     * @fn 向容器中添加子组件
     * @param control 子组件
     */
    void Add           (Control *control)   noexcept;
    /**
     * @fn 从容器中移除子组件
     * @param control 子组件
     */
    void Remove        (Control *control)   noexcept;
    /**
     * @fn 释放所有子组件
     * @note 所有组组件地址都将被释放，请注意空指针。
     */
    void FreeAll       ()                   noexcept;
    /**
     * @fn 隐藏所有子组件
     * @note 无后效方法。
     */
    void HideAll       ()                   noexcept;
    /**
     * @fn 显示所有子组件
     * @note 无后效方法。
     */
    void ShowAll       ()                   noexcept;
    /**
     * @fn 切换所有子组件的可见性
     * @note 无后效方法。
     */
    void ToggleVisible ()                   noexcept;
    /**
     * @fn 释放所有没隐藏着的子组件
     * @note 无后效方法。
     */
    void FreeAllVisible()                   noexcept;
    /**
     * @fn 同步子组件
     * @param pointer 外部子组件
     * @warning 务必监控该容器对外部子组件的行为。
     * @note 不同步外部子组件的父容器。
     * @note 会先释放所有原子组件。
     */
    void SyncChildren(Children *pointer)  noexcept;
    /**
     * @fn 取消同步子组件
     * @warning 请注意检查内存泄漏。
     */
    void UnsyncChildren()                 noexcept;
    
    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetIgnoreOutside(bool flag) noexcept;
    /**
     * @fn 释放所有隐藏着的子组件
     * @note 无后效方法。
     */
    void FreeAllHiden    ()          noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update (bool resetMinSize = true)                                 noexcept = 0;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)   noexcept;
    void Draw   (sf::RenderWindow &screen)                                 noexcept;
    void Tick   ()                                                         noexcept;

    ~Container() noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void UpdateByDefault(Direction direction, Control *child) noexcept;
    void UpdateSizeByDefault    (Direction direction, Control *child) noexcept;
    void UpdatePositionByDefault(Direction direction, Control *child) noexcept;
    static unsigned int ObtainDefaultGlobalSize    (Direction direction, const Control *child, unsigned int parentSize)                                                              noexcept;
    static int          ObtainDefaultGlobalPosition(Direction direction, const Control *child, unsigned int parentSize, int parentPosition)                                          noexcept;
    static unsigned int ObtainDefaultGlobalSize    (ValueType sizeValueType, bool wrap,   unsigned int size,     unsigned int minSize,  unsigned int parentSize)                     noexcept;
    static int          ObtainDefaultGlobalPosition(int       center,        int  anchor, int          position, unsigned int thisSize, unsigned int parentSize, int parentPosition) noexcept;
    bool IsThisInside(const Control *child) noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */
    Children *children = new Children{};
    
    /********************
     * @brief 样式属性。*
     * ******************
     */
    bool ignoreOutside = false;
};

class Flat : public Container{
public:
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class Screen : public Flat{
public:
    Screen() noexcept
    {
        unsigned int width = globalSize[Direction::HORIZONTAL];
        unsigned int height = globalSize[Direction::VERTICAL];
        screen.create(sf::VideoMode({width, height}), "Caption");
        screen.setVerticalSyncEnabled(true);
    }
    Screen(unsigned int width, unsigned int height, const sf::String &caption) noexcept
    {
        screen.create(sf::VideoMode({width, height}), caption);
        screen.setVerticalSyncEnabled(true);
        SetGlobalSize(Direction::HORIZONTAL, width);
        SetGlobalSize(Direction::VERTICAL, height);
    }
    /**
     * @fn 获取只读窗口实例
     * @return 只读窗口实例
     */
    const sf::RenderWindow &Get() const noexcept { return screen; }
    /**
     * @fn 设置窗口大小
     * @param width 宽度
     * @param height 高度
     */
    void SetRange  (unsigned int width, unsigned int height) noexcept;
    /**
     * @fn 设置窗口标题
     * @param caption 标题
     */
    void SetCaption(const sf::String &caption)               noexcept;
    /**
     * @fn 检查窗口是否打开
     * @return 是否打开
     */
    bool IsOpen    ()                                  const noexcept;
    /**
     * @fn 刷新窗口
     * @note 务必在事件循环中调用。
     */
    void Tick      ()                                        noexcept;
protected:
    sf::RenderWindow screen;
};

class Center : public Container{
public:
    Center() noexcept
    {
        UpdateInQueue(true);
    }
    
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */
    
    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class LinearBox : public Container{
public:
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    /**
     * @fn 设置子组件 wrap 属性
     * @param direction 方向
     * @param flag 是否 wrap
     * @note 无后效方法。
     */
    void SetAllChildrenWrap(Direction direction, bool flag) noexcept;

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetGap(int absolute)         noexcept;

    void SetProportionMode(bool flag) noexcept;
    void SetDelta(int absolute)       noexcept;
    unsigned int GetTotalSize() const noexcept { return totalSize; }

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept = 0;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void UpdateLinear(Direction direction, bool resetMinSize) noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    int gap = 25;

    bool proportionMode = true;
    int delta = 0;
    unsigned int totalSize = 0;

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class HorizontalBox : public LinearBox{
public:
    HorizontalBox() noexcept
    {
        UpdateInQueue(true);
    }
    
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class VerticalBox : public LinearBox{
public:
    VerticalBox() noexcept
    {
        UpdateInQueue(true);
    }
    
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class Margin : public Container{
public:
    Margin() noexcept
    {
        UpdateInQueue(true);
    }
    
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetMargin(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right) noexcept;
    void SetMarginTop   (unsigned int top)    noexcept;
    void SetMarginBottom(unsigned int bottom) noexcept;
    void SetMarginLeft  (unsigned int left)   noexcept;
    void SetMarginRight (unsigned int right)  noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
private:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    struct{
        unsigned int top = 25;
        unsigned int bottom = 25;
        unsigned int left = 25;
        unsigned int right = 25;
    }margin;

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class Label : public Control{
    static const sf::String FONT_FILE_PATH;
public:
    Label() noexcept
    {
        SetFont(FONT_FILE_PATH);
        UpdateInQueue(true);
    }
    const sf::String &GetContent() const noexcept { return content; }
    bool              GetError()   const noexcept { return error; }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetContent(const sf::String &newContent) noexcept;
    void SetFontSize(unsigned int size)           noexcept;
    void SetFont(const sf::String &fontFile)      noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetFontColor(const sf::Color &color)       noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update (bool resetMinSize = true) noexcept;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)   noexcept {}
    void Draw   (sf::RenderWindow &screen) noexcept;
protected:
    static const unsigned int REVISION_X;
    static const unsigned int REVISION_Y;

    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    sf::String content = "";
    unsigned int fontSize = 50;
    sf::Font font;
    sf::Text text;

    /********************
     * @brief 样式属性。*
     * ******************
     */
    sf::Color fontColor = sf::Color::White;
    bool error = false;

};

class Spacer : public Control{
public:
    Spacer() noexcept
    {
        rect.setOutlineThickness(0);
        rect.setOutlineColor(sf::Color::Transparent);
        rect.setFillColor(sf::Color::Transparent);
    }
    
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetOutlineThickness(float thickness)    noexcept;
    void SetOutlineColor(const sf::Color &color) noexcept;
    void SetFillColor(const sf::Color &color)    noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update (bool resetMinSize = true) noexcept;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)   noexcept {}
    void Draw   (sf::RenderWindow &screen) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */

    sf::RectangleShape rect;
};

class Button : public Control{
public:
    Button() noexcept
    {
        rect->SetPreset(Preset::FILL_FROM_CENTER);
        rect->SetOutlineThickness(5);
        label->SetPreset(Preset::WRAP_AT_CENTER);
        layer.Add(rect);
        layer.Add(label);
        UpdateInQueue(true);
    }
    void SetEnterCallback(Callback function)     noexcept { enterCallback = function; }
    void SetLeaveCallback(Callback function)     noexcept { leaveCallback = function; }
    void SetPressDownCallback(Callback function) noexcept { pressDownCallback = function; }
    void SetPressUpCallback(Callback function)   noexcept { pressUpCallback = function; }
    void SetClickCallback(Callback function)     noexcept { clickCallback = function; }
    const sf::String &GetCaption() const noexcept { return label->GetContent(); }
    bool              GetEntered() const noexcept { return entered; }
    bool              GetPressed() const noexcept { return pressed; }
    bool              GetError()   const noexcept { return label->GetError(); }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetCaption(const sf::String &caption)   noexcept;
    void SetFontSize(unsigned int size)            noexcept;
    void SetFont(const sf::String &fontFile)      noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetFontColor(const sf::Color &color)      noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update (bool resetMinSize = true) noexcept;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)   noexcept;
    void Draw   (sf::RenderWindow &screen) noexcept;
    void Tick() noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void SetEntered(bool flag) noexcept;
    void SetPressed(bool flag) noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    Spacer *rect = new Spacer;
    Label *label = new Label;
    Flat layer;

    /********************
     * @brief 样式属性。*
     * ******************
     */
    bool entered = false;
    bool pressed = false;

    Callback enterCallback = DO_NOTHING;
    Callback leaveCallback = DO_NOTHING;
    Callback pressDownCallback = DO_NOTHING;
    Callback pressUpCallback = DO_NOTHING;
    Callback clickCallback = DO_NOTHING;
};

class InputBox : public Control{
public:
    InputBox() noexcept
    {
        button->SetPreset(Preset::FILL_FROM_CENTER);
        button->SetClickCallback([this](const sf::String &name, const sf::Event &event){
            if (!this->inputting) {
                this->SetInputting(true);
                this->beginCallback(this->name, event);
            }
        });
        rect->SetPreset(Preset::FILL_FROM_CENTER);
        label->SetPreset(Preset::FILL_FROM_CENTER);
        layer.Add(button);
        layer.Add(rect);
        layer.Add(label);
        UpdateInQueue(true);
    }
    void SetBeginCallback(Callback function)  noexcept { beginCallback = function; }
    void SetInputCallback(Callback function)  noexcept { inputCallback = function; }
    void SetEndCallback(Callback function)    noexcept { endCallback = function; }
    void SetExceedLimitCallback(Callback function) noexcept { exceedLimitCallback = function; }
    const sf::String &GetText() const noexcept { return textCopy; }
    bool GetInputting()         const noexcept { return inputting; }
    bool GetError()             const noexcept { return button->GetError() || label->GetError(); }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    enum class ContentLimit{
        NONE, 
        BAN_SPECIAL_CHARACTERS,
        ALLOW_SPECIAL_CHARACTERS_ONLY,
    };
    static const sf::String NUMBER;
    static const sf::String LOWER_LETTER;
    static const sf::String UPPER_LETTER;
    static const sf::String ASCII;

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetText(const sf::String &text)           noexcept;
    void SetFontSize(unsigned int size)            noexcept;
    void SetFont(const sf::String &fontFile)       noexcept;
    void SetProtectText(bool flag)               noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetFontColor(const sf::Color &color)          noexcept;
    void SetInputtingFontColor(const sf::Color &color) noexcept;
    void SetBackColor(const sf::Color &color)          noexcept;
    void SetInputtingBackColor(const sf::Color &color) noexcept;
    void SetFlickerInterval(unsigned int interval)     noexcept;
    void SetCursorThickness(float thickness)           noexcept;
    void SetLengthLimit(unsigned int maxLength)              noexcept;
    void SetContentLimit(ContentLimit limit)                 noexcept;
    void SetSpecialCharacters(const sf::String &list) noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update (bool resetMinSize = true)                                 noexcept;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)   noexcept;
    void Draw   (sf::RenderWindow &screen)                                 noexcept;
    void Tick   ()                                                         noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void SetInputting(bool flag) noexcept;
    bool IsSpecialCharacter(wchar_t c) noexcept;
    sf::String GetTextSatisfysLimits(const sf::String &s) noexcept;
    bool cursorVisible = true;
    sf::Clock flickerTimer;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    Button *button = new Button;
    Spacer *rect   = new Spacer;
    Label  *label  = new Label;
    Flat layer;
    sf::String textCopy = "";
    bool protectText    = false;

    /********************
     * @brief 样式属性。*
     * ******************
     */
    bool inputting = false;
    sf::Color fontColor          = sf::Color::White;
    sf::Color backColor          = sf::Color::Transparent;
    sf::Color inputtingFontColor = sf::Color::Black;
    sf::Color inputtingBackColor = sf::Color::White;
    unsigned int flickerInterval = 500;
    float        cursorThickness = 2;
    unsigned int lengthLimited = 0;
    ContentLimit contentLimit  = ContentLimit::NONE;
    sf::String specialCharacters = "";

    Callback beginCallback = DO_NOTHING;
    Callback inputCallback = DO_NOTHING;
    Callback endCallback = DO_NOTHING;
    Callback exceedLimitCallback = DO_NOTHING;
};

class ScrollBar : public Control{
public:
    ScrollBar() noexcept
    {
        background->SetPreset(Preset::FILL_FROM_CENTER);
        background->SetOutlineThickness(5);
        background->SetOutlineColor(backColor);
        front->SetOutlineThickness(5);
        front->SetOutlineColor(frontColor);

        background->SetFillColor(sf::Color::Transparent);
        front->SetFillColor(backColor);
        
        layer.Add(background);
        layer.Add(front);
    }
    void SetEnteredCallback(Callback function) noexcept { enteredCallback = function; }
    void SetLeaveCallback  (Callback function) noexcept { leaveCallback = function; }
    void SetScrollCallback (Callback function) noexcept { scrollCallback = function; }
    void SetDragCallback (Callback function) noexcept { dragCallback = function; }
    unsigned int GetPort() const noexcept { return port; }
    unsigned int GetSum()  const noexcept { return sum; }
    unsigned int GetRate() const noexcept { return rate; }
    bool GetEntered()  const noexcept { return entered; }
    bool GetDragging() const noexcept { return dragging; }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetPort(unsigned int absolute) noexcept;
    void SetSum (unsigned int absolute) noexcept;
    void SetRate(unsigned int absolute) noexcept;
    void AddRate(int absolute)          noexcept;
    void Scroll (float delta)           noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetSensitivity(float real)               noexcept;
    void SetReserve    (bool flag)                noexcept;
    void SetBackColor  (const sf::Color &color)   noexcept;
    void SetFrontColor (const sf::Color &color)   noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update (bool resetMinSize = true)                                 noexcept = 0;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)   noexcept;
    void Draw   (sf::RenderWindow &screen)                                 noexcept;
    void Tick   ()                                                         noexcept;
protected:
    Callback enteredCallback = DO_NOTHING;
    Callback leaveCallback = DO_NOTHING;
    Callback scrollCallback = DO_NOTHING;
    Callback dragCallback = DO_NOTHING;

    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    virtual void DragTo (int x, int y) noexcept = 0;
    void UpdateTo(Direction direction) noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    unsigned int sum  = 100;
    unsigned int rate = 20;
    unsigned int port = 10;
    Spacer *background = new Spacer;
    Spacer *front      = new Spacer;
    Flat layer;

    /********************
     * @brief 样式属性。*
     * ******************
     */
    float sensitivity = 2;
    bool reserve = false;
    sf::Color backColor = sf::Color::White;
    sf::Color frontColor = sf::Color::Blue;
    bool entered  = false;
    bool dragging = false;
};

class HorizontalScrollBar : public ScrollBar{
public:
    HorizontalScrollBar() noexcept
    {
        UpdateInQueue();
    }
    
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void DragTo(int x, int y)             noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class VerticalScrollBar : public ScrollBar{
public:
    VerticalScrollBar() noexcept
    {
        UpdateInQueue();
    }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void DragTo(int x, int y)             noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class ScrollingBox : public Container{
public:
    ScrollingBox() noexcept
    {
        SetIgnoreOutside(true);

        rect->SetOutlineThickness(5);
        rect->SetOutlineColor(sf::Color::White);

        layer.Add(rect);
    }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetBarSize(unsigned int size) noexcept;
    void SetBarAtFront(bool flag) noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetRate            (unsigned int   absolute)  noexcept;
    void SetSensitivity     (float          real)      noexcept;
    void SetReserve         (bool           flag)      noexcept;
    void SetInsideBoxScrollable(bool           flag)      noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true)                                 noexcept = 0;
    void Process(const sf::Event &event, const sf::RenderWindow &screen)  noexcept;
    void Draw(sf::RenderWindow &screen)                                   noexcept;
    void Tick()                                                           noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void SharedUpdate(bool resetMinSize, Direction direction) noexcept;
    virtual LinearBox *GetBox() const noexcept = 0;
    virtual ScrollBar *GetBar() const noexcept = 0;
    bool enteredBox = false;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    static const float DEFAULT_SENSITIVITY;
    
    /********************
     * @brief 内容属性。*
     * ******************
     */
    unsigned int barSize = 50;
    bool barAtFront = false;
    Spacer *rect = new Spacer;
    Flat layer;
    
    /********************
     * @brief 样式属性。*
     * ******************
     */
    bool insideScrollable = false;
};

class HorizontalScrollingBox : public ScrollingBox{
public:
    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    HorizontalScrollingBox() noexcept
    {
        box->SetProportionMode(false);
        box->SyncChildren(children);
        
        bar->SetScrollCallback([this](const sf::String &name, const sf::Event &event){
            box->SetDelta(-bar->GetRate());
        });
        SetSensitivity(DEFAULT_SENSITIVITY);

        box->SetHPreset(Preset::FILL_FROM_FRONT);
        bar->SetHPreset(Preset::FILL_FROM_FRONT);

        layer.Add(box);
        layer.Add(bar);

        UpdateInQueue(true);
    }
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true)                                 noexcept;

    ~HorizontalScrollingBox() noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    LinearBox *GetBox() const noexcept override { return box; }
    ScrollBar *GetBar() const noexcept override { return bar; }

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */
    HorizontalBox *box = new HorizontalBox;
    HorizontalScrollBar *bar = new HorizontalScrollBar;
    
    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class VerticalScrollingBox : public ScrollingBox{
public:
    VerticalScrollingBox() noexcept
    {
        box->SetProportionMode(false);
        box->SyncChildren(children);

        bar->SetScrollCallback([this](const sf::String &name, const sf::Event &event){
            box->SetDelta(-bar->GetRate());
        });
        bar->SetSensitivity(DEFAULT_SENSITIVITY);

        box->SetVPreset(Preset::FILL_FROM_FRONT);
        bar->SetVPreset(Preset::FILL_FROM_FRONT);

        layer.Add(box);
        layer.Add(bar);

        UpdateInQueue(true);
    }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;

    ~VerticalScrollingBox() noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    LinearBox *GetBox() const noexcept override { return box; }
    ScrollBar *GetBar() const noexcept override { return bar; }

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */
    
    /********************
     * @brief 内容属性。*
     * ******************
     */
    VerticalBox *box = new VerticalBox;
    VerticalScrollBar *bar = new VerticalScrollBar;
    
    /********************
     * @brief 样式属性。*
     * ******************
     */
};

class LoadingRing : public Control{
public:
    LoadingRing() noexcept
    {
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(10);
        UpdateInQueue();
    }
    void SetCallback(Callback function) noexcept { callback = function; }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */
    static const float PI;
    
    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetColor(const sf::Color &color) noexcept;
    void SetThickness(float thickness) noexcept;
    void SetSpeed(float speed) noexcept;
    void SetInterval(unsigned int interval) noexcept;
    void Start() noexcept;
    void Stop()  noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept {}
    void Process(const sf::Event &event, const sf::RenderWindow &screen) noexcept {}
    void Draw(sf::RenderWindow &screen) noexcept;
protected:
    Callback callback = DO_NOTHING;

    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    float rate = 0;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    sf::CircleShape circle;

    /********************
     * @brief 样式属性。*
     * ******************
     */
    float speed = 0.05;
    unsigned int interval = 200;
    sf::Clock clock;
    bool started = false;
};

class LoadingRingWithText : public Control {
public:
    LoadingRingWithText() noexcept
    {
        layer.Add(label);
        label->SetPreset(Direction::HORIZONTAL, Preset::FILL_FROM_CENTER);
        label->SetSize(Direction::VERTICAL, textHeight);
        layer.Add(ring);
        ring->SetPreset(Direction::HORIZONTAL, Preset::FILL_FROM_CENTER);
        ring->SetSize(Direction::VERTICAL, ringHeight);
        ring->SetInterval(1000);
        ring->SetCallback([this](const sf::String &name, const sf::Event &event){
            this->Count();
        });
        UpdateInQueue(true);
    }
    void SetCountCallback(Callback function) noexcept { countCallback = function; }
    void SetFinishedCallback(Callback function) noexcept { finishedCallback = function; }
    int  GetCount()   const noexcept { return count; }
    bool GetError()   const noexcept { return label->GetError(); }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetFontSize(unsigned int size)           noexcept;
    void SetFont(const sf::String &fontFile)      noexcept;
    void SetTextHeight(unsigned int height)       noexcept;
    void SetRingHeight(unsigned int height)       noexcept;
    void SetText(const sf::String &newText)       noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */
    void SetCount(int newCount)                     noexcept;
    void SetFontColor(const sf::Color &color)       noexcept;
    void SetInterval(unsigned int interval)         noexcept;
    void Start() noexcept;
    void Stop()  noexcept;

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
    void Process(const sf::Event &event, const sf::RenderWindow &screen) noexcept;
    void Draw(sf::RenderWindow &screen) noexcept;
    void Tick() noexcept;
protected:
    Callback countCallback = DO_NOTHING;
    Callback finishedCallback = DO_NOTHING;

    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */
    void Count() noexcept;

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    VerticalBox layer;
    Label *label = new Label;
    LoadingRing *ring = new LoadingRing;
    unsigned int textHeight = 140;
    unsigned int ringHeight = 60;
    sf::String text = "";

    /********************
     * @brief 样式属性。*
     * ******************
     */
    int count = 5;
};

class PictureBox : public Control {
public:
    PictureBox() noexcept
    {
        ;
    }
    bool GetError() const noexcept { return error; }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */
    void SetPicture(const sf::String &filename) noexcept;
    void SetScale(unsigned int percentage) noexcept;
    void KeepWidth(unsigned int absolute) noexcept;
    void KeepHeight(unsigned int absolute) noexcept;

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept;
    void Process(const sf::Event &event, const sf::RenderWindow &screen) noexcept {}
    void Draw(sf::RenderWindow &screen) noexcept;
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */
    sf::Texture texture;
    struct{
        unsigned int width = 200;
        unsigned int height = 100;
    }originSize;
    sf::Sprite sprite;

    /********************
     * @brief 样式属性。*
     * ******************
     */
    bool error = false;
};
    
class Example : public Control {
public:
    Example() noexcept
    {
        ;
    }

    /*******************************************
     * @brief 约定的常量和数据类型以及外工具方法。*
     * *****************************************
     */

    /***************************
     * @brief 内容属性控制接口。*
     * *************************
     */

    /***************************
     * @brief 样式属性控制接口。*
     * *************************
     */

    /************************************
     * @brief 实现了的和待实现的抽象方法。*
     * **********************************
     */
    void Update(bool resetMinSize = true) noexcept = 0;
    void Process(const sf::Event &event, const sf::RenderWindow &screen) noexcept = 0;
    void Draw(sf::RenderWindow &screen) noexcept = 0;
    void Tick() noexcept = 0; // 组合时需重写
protected:
    /******************************
     * @brief 封装的工具方法和属性。*
     * ****************************
     */

    /******************************
     * @brief 封装的数据类型和常量。*
     * ****************************
     */

    /********************
     * @brief 内容属性。*
     * ******************
     */

    /********************
     * @brief 样式属性。*
     * ******************
     */
};

}

#endif
