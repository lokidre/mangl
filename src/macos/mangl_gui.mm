#import "mangl_gui.h"

#import "mangl_main_vc.h"

#include "../core/mangl_environment.h"
#include "../core/native/mangl_native_gui.h"
#include "../apple/mangl_imacos_util.h"

#include "mangl_main_vc.h"


using namespace mangl;

enum class MWidgetType { UNKNOWN, LABEL, INPUT };


struct MGuiWidgetContext__ {
    WidgetId key;
    MWidgetType type{MWidgetType::UNKNOWN};
    mangl::Rect frame;
    NSView *widget;
    mangl::String currentText;
};

static std::unordered_map<WidgetId, MGuiWidgetContext__> widgets_;


@interface ManglGuiHelper() <NSTextFieldDelegate> {
}

@end


@implementation ManglGuiHelper   // NSTextFieldDelegate

+ (instancetype)instance
{
    static ManglGuiHelper *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
      sharedInstance = [[ManglGuiHelper alloc] init];
      [sharedInstance ctor];
    });

    return sharedInstance;
}

- (void)ctor
{
    // screenSize_.w = MGameRuntimeViewController.instance.sceneView.frame.size.width ;
    // screenSize_.h = MGameRuntimeViewController.instance.sceneView.frame.size.height ;
}

- (int)generateWidgetKey
{
    static int widgetKey_ = 1;
    int key = widgetKey_++;
    return key;
}

- (MGuiWidgetContext__ &)reserveNewWidget
{
    MGuiWidgetContext__ ctx;
    ctx.key = [self generateWidgetKey];
    // ctx.widget = widget ;
    widgets_[ctx.key] = ctx;
    return widgets_[ctx.key];
}

- (MGuiWidgetContext__ &)guiAddNewWidget:(NSView *)widget forKey:(WidgetId)key
{
    // widget.tag = key ;
    [ManglMainViewController.instance.view addSubview:widget];

    widgets_[key].widget = widget;
    return widgets_[key];
}

- (void)removeWidget:(int)widgetId
{
    if (auto it = widgets_.find(widgetId); it != widgets_.end()) {
        [it->second.widget removeFromSuperview];
        widgets_.erase(it);
    }
}

- (NSView *)guiFindWidget:(WidgetId)widgetId
{
    if (auto it = widgets_.find(widgetId); it != widgets_.end())
        return it->second.widget;
    return nil;
}

- (MGuiWidgetContext__ &)findContext:(WidgetId)widgetId
{
    return widgets_[widgetId];
}


//+ (Cord)nativeGuiCoordsK
//{
//    return Env::upscale / Env::pixelDensity;
//}

- (CGRect)toGuiRect:(RectA)rect
{
    auto screen = Env::toScreen(rect);
    //Cord guiK = ManglGuiHelper.nativeGuiCoordsK;   // mangl_environment_->upscale / mangl_environment_->pixelDensity ;
    // MCoord k = 1 ; //.0 / mangl_environment_->pixelDensity ; /// mangl_environment_->upscale ;
    //rect *= guiK;   // .scale(k) ;

    // return CGRectMake ( rect.x, screenSize_.h - rect.y - rect.h,  rect.w, rect.h ) ;
    return CGRectMake(screen.x, screen.y, screen.w, screen.h);
}


- (void)clearWidgets
{
    dispatch_async(dispatch_get_main_queue(), ^{
      for (auto it = widgets_.begin(); it != widgets_.end();) {
          [it->second.widget removeFromSuperview];
          it = widgets_.erase(it);
      }
      widgets_.clear();
    });
}

- (WidgetId)createInput:(NSString *)text frame:(MRect)frame flags:(Flags)flags
{
    auto &ctx = [self reserveNewWidget];
    auto key = ctx.key;

    dispatch_async(dispatch_get_main_queue(), ^{
        auto widget = [[NSText alloc] initWithFrame:[self toGuiRect:frame]];
        // widget.delegate = self ;
        widget.string = text;

        if (flags & M::AlignCenter)
            widget.alignment = NSTextAlignmentCenter;
        if (flags & M::AlignRight)
            widget.alignment = NSTextAlignmentRight;

        auto &ctx = [self guiAddNewWidget:widget forKey:key];

        ctx.type = MWidgetType::INPUT;
        ctx.frame = frame;
    });

    return key;
}

- (WidgetId)createLabel:(NSString *)text frame:(mangl::Rect)frame flags:(Flags)flags
{
    auto &ctx = [self reserveNewWidget];
    auto key = ctx.key;

    dispatch_async(dispatch_get_main_queue(), ^{
        auto widget = [[NSText alloc] initWithFrame:[self toGuiRect:frame]];
        // widget.bezeled = false ;
        widget.drawsBackground = false;
        widget.editable = false;
        widget.selectable = false;

        widget.string = text;

        if (flags & M::AlignCenter)
            widget.alignment = NSTextAlignmentCenter;
        if (flags & M::AlignRight)
            widget.alignment = NSTextAlignmentRight;


        auto &ctx = [self guiAddNewWidget:widget forKey:key];

        ctx.type = MWidgetType::LABEL;
        ctx.frame = frame;
    });

    return key;
}



- (void)setInputPlaceholder:(WidgetId)widgetId text:(NSString *)text
{
    // auto widget = (NSText *)[self guiFindWidget:widgetId] ;
    // widget.placeholder = text ;
}

- (void)setTextColor:(WidgetId)widgetId color:(mangl::Color)color
{
    dispatch_async(dispatch_get_main_queue(), ^{
      auto &ctx = [self findContext:widgetId];
      if (ctx.type == MWidgetType::INPUT || ctx.type == MWidgetType::LABEL)
          ((NSText *)ctx.widget).textColor = toNSColor(color);
    });
}


- (void)setBackgroundColor:(WidgetId)widgetId color:(mangl::Color)color
{
    //auto widget = [self guiFindWidget:widgetId];
    // widget.backgroundColor = toNSColor(color) ;
}

- (void)setText:(WidgetId)widgetId text:(NSString *)text
{
    dispatch_async(dispatch_get_main_queue(), ^{
        auto &ctx = [self findContext:widgetId];
        if (ctx.type == MWidgetType::INPUT || ctx.type == MWidgetType::LABEL)
            ((NSText *)ctx.widget).string = text;
    });
}

- (NSString *)getText:(WidgetId)widgetId
{
    auto &ctx = [self findContext:widgetId];
    if (ctx.type == MWidgetType::INPUT || ctx.type == MWidgetType::LABEL)
        return ((NSText *)ctx.widget).string;
    return nil;
}

- (void)setVisible:(WidgetId)widgetId visible:(bool)visible
{
    dispatch_async(dispatch_get_main_queue(), ^{
      auto widget = [self guiFindWidget:widgetId];
      widget.hidden = !visible;
      //[widget setNeedsDisplay] ;
    });
}

- (void)moveWidget:(WidgetId)widgetId frame:(MRect)frame
{
    dispatch_async(dispatch_get_main_queue(), ^{
      auto widget = [self guiFindWidget:widgetId];
      widget.frame = [self toGuiRect:frame];
      //[widget setNeedsDisplay] ;
    });
}

- (void)moveWidget:(WidgetId)widgetId point:(mangl::Point)point
{
    // TODO:
}


- (void)setFont:(WidgetId)widgetId size:(Cord)size fontName:(NSString *)fontName
{
    auto guiK = Env::pixelDensity;
    auto font = [NSFont controlContentFontOfSize:size * guiK];
    auto &ctx = [self findContext:widgetId];
    if (ctx.type == MWidgetType::INPUT || ctx.type == MWidgetType::LABEL)
        [((NSText *)ctx.widget) setFont:font];
}


//
// MARK: NSTextFieldDelegate
//
//- (BOOL)textFieldShouldReturn:(NSTextField *)textField
//{
//	[textField resignFirstResponder] ;
//	return YES ;
//}

@end

//
// MARK: MNativeGui
//
namespace mangl {

void NativeGui::alert(AlertParamCR parms)
{
    auto alert = [[NSAlert alloc] init];
    [alert setShowsHelp:NO];
    [alert setAlertStyle:NSAlertStyleInformational];
    [alert setMessageText:toNSString(parms.title)];
    [alert setInformativeText:toNSString(parms.message)];
    [alert runModal];
}

auto NativeGui::clearWidgets() -> SelfRet {
    [ManglGuiHelper.instance clearWidgets];
    return this;
}

WidgetId NativeGui::createInput(StrView text, RectA frame, Flags flags) {
    return [ManglGuiHelper.instance createInput:toNSString(text) frame:frame flags:flags];
}

WidgetId NativeGui::createLabel(StrView text, RectA frame, Flags flags) {
    return [ManglGuiHelper.instance createLabel:toNSString(text) frame:frame flags:flags];
}

void NativeGui::setInputPlaceholder(WidgetId widgetId, StrView text) {
    [ManglGuiHelper.instance setInputPlaceholder:widgetId text:toNSString(text)];
}

auto NativeGui::setText(WidgetId widgetId, StrView text) -> SelfRet {
    [ManglGuiHelper.instance setText:widgetId text:toNSString(text)];
    return this;
}

String NativeGui::getText(WidgetId widgetId) {
    // return [MGuiHelper.instance getText:widgetId].UTF8String ;
    return widgets_[widgetId].currentText;
}


auto NativeGui::setVisible(WidgetId widgetId, bool visible) -> SelfRet {
    [ManglGuiHelper.instance setVisible:widgetId visible:visible];
    return this;
}

auto NativeGui::move(WidgetId widgetId, RectA frame) -> SelfRet {
    [ManglGuiHelper.instance moveWidget:widgetId frame:frame];
    return this;
}

auto NativeGui::move(WidgetId widgetId, mangl::Point pos) -> SelfRet {
    [ManglGuiHelper.instance moveWidget:widgetId point:pos];
    return this;
}


auto NativeGui::setTextColor(WidgetId widgetId, ColorA color) -> SelfRet {
    [ManglGuiHelper.instance setTextColor:widgetId color:color];
    return this;
}

auto NativeGui::setBackgroundColor(WidgetId widgetId, ColorA color) -> SelfRet {
    [ManglGuiHelper.instance setBackgroundColor:widgetId color:color];
    return this;
}

auto NativeGui::setFont(WidgetId widgetId, Cord size, StrView fontName) -> SelfRet {
    [ManglGuiHelper.instance setFont:widgetId size:size fontName:(fontName.empty() ? nil : toNSString(fontName))];
    return this;
}

}   // namespace mangl
