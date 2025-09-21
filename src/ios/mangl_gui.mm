#import "mangl_gui.h"

#import "mangl_main_vc.h"

#include "../core/mangl_common.h"
#include "../core/mangl_util.h"

#include "../apple/mangl_imacos_util.h"

namespace mangl {

enum class WidgetType { Unknown, Label, Input };

struct GuiWidgetContext {
    WidgetId key;
    WidgetType type{WidgetType::Unknown};
    Rect frame;
    UIView* widget;
};

}   // namespace mangl


using namespace mangl;

@interface ManglGuiHelper () {
    std::unordered_map<WidgetId, std::unique_ptr<GuiWidgetContext>> widgets_;
    //Env* env_;
}

@end


@implementation ManglGuiHelper

+ (instancetype)instance {
    static ManglGuiHelper *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglGuiHelper alloc] init];
        [sharedInstance ctor];
    });

    return sharedInstance;
}

- (void)ctor {
}

- (GuiWidgetContext *)guiAddNewWidget:(UIView *)widget {
    
    // static int widgetKey_ = 1;
    auto ctxp = std::make_unique<GuiWidgetContext>();
    auto ctx = ctxp.get();

    auto key = M::generateId<WidgetId>();

    // int key = widgetKey_++;
    widget.tag = (NSInteger)key;
    [ManglMainViewController.instance.renderView addSubview:widget];

    ctx->key = key;
    ctx->widget = widget;

    widgets_.insert({key, std::move(ctxp)});
    return ctx;
}

- (void)removeWidget:(WidgetId)widgetId
{
    if (auto it = widgets_.find(widgetId); it != widgets_.end()) {
        [it->second->widget removeFromSuperview];
        widgets_.erase(it);
    }
}

- (UIView *)guiFindWidget:(WidgetId)widgetId
{
    if (auto it = widgets_.find(widgetId); it != widgets_.end())
        return it->second->widget;
    return nil;
}

- (GuiWidgetContext *)findContext:(WidgetId)widgetId {
    return widgets_[widgetId].get();
}


- (WidgetId)inputDialog:(NativeGui::InputDialogParam)param
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto alert = [UIAlertController alertControllerWithTitle:toNSString(param.title)
                                                         message:toNSString(param.prompt)
                                                  preferredStyle:UIAlertControllerStyleAlert];
        
        [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
            textField.placeholder = toNSString(param.placeholder);
            textField.text = toNSString(param.text);
        }];
        
        
        auto okAction = [UIAlertAction actionWithTitle:toNSString(param.ok)
                                                 style:UIAlertActionStyleDefault
                                               handler:^(UIAlertAction * _Nonnull action) {
            if (param.signal) {
                bool close = false;
                param.signal(true, toStringView(alert.textFields[0].text), &close);
            }
        }];
        
        if (!param.cancel.empty()) {
            auto cancelAction = [UIAlertAction actionWithTitle:toNSString(param.cancel)
                                                         style:UIAlertActionStyleDefault
                                                       handler:^(UIAlertAction * _Nonnull action) {
                if (param.signal) {
                    bool close = false;
                    param.signal(false, toStringView(alert.textFields[0].text), &close);
                }
            }];
            [alert addAction:cancelAction];
        }
        
        [alert addAction:okAction];
        [ManglMainViewController.instance presentViewController:alert animated:YES completion:nil];
    });
    
    return 0;  // Think about some Id
}


- (void)alert:(NSString *)message withTitle:(NSString *)title
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto alert = [UIAlertController alertControllerWithTitle:title
                                                         message:message
                                                  preferredStyle:UIAlertControllerStyleAlert];
        
        auto action = [UIAlertAction actionWithTitle:@"OK" 
                                               style:UIAlertActionStyleDefault
                                             handler:^(UIAlertAction *action) {
            NativeGui::instance()->onAlertConfirmed(0);
        }];
        
        [alert addAction:action];
        [ManglMainViewController.instance presentViewController:alert animated:YES completion:nil];
    });
}

- (void)clearWidgets
{
    for (auto it = widgets_.begin(); it != widgets_.end();) {
        [it->second->widget removeFromSuperview];
        it = widgets_.erase(it);
    }
    widgets_.clear();
}

- (WidgetId)createInput:(NSString *)text frame:(RectA)frame flags:(Flags)flags
{
    auto widget = [[UITextField alloc] initWithFrame:toCGRect(Env::toScreen(frame))];
    widget.delegate = self;
    widget.text = text;

    if (flags & M::AlignCenter)
        widget.textAlignment = NSTextAlignmentCenter;
    else if (flags & M::AlignRight)
        widget.textAlignment = NSTextAlignmentRight;

    widget.keyboardType = UIKeyboardTypeDefault;
    widget.returnKeyType = UIReturnKeyDone;

    auto ctx = [self guiAddNewWidget:widget];

    ctx->type = WidgetType::Input;
    ctx->frame = frame;

    return ctx->key;
}

- (WidgetId)createLabel:(NSString *)text frame:(RectA)frame flags:(Flags)flags
{
    auto widget = [[UILabel alloc] initWithFrame:toCGRect(Env::toScreen(frame))];
    widget.text = text;

    if (flags & M::AlignCenter)
        widget.textAlignment = NSTextAlignmentCenter;
    else if (flags & M::AlignRight)
        widget.textAlignment = NSTextAlignmentRight;

    auto ctx = [self guiAddNewWidget:widget];

    ctx->type = WidgetType::Label;
    ctx->frame = frame;

    return ctx->key;
}


- (void)setInputPlaceholder:(WidgetId)widgetId text:(NSString *)text
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto widget = (UITextField *)[self guiFindWidget:widgetId];
        widget.placeholder = text;
        
    });
}

- (void)setTextColor:(WidgetId)widgetId color:(ColorA)color
{
    auto uiColor = toUIColor(color);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto ctx = [self findContext:widgetId];
        if (ctx->type == WidgetType::Input) {
            auto textField = objc_cast<UITextField>(ctx->widget);
            MDebugTrapAssert(textField);
            textField.textColor = uiColor;
        } else if (ctx->type == WidgetType::Label) {
            auto label = objc_cast<UILabel>(ctx->widget);
            MDebugTrapAssert(label);
            label.textColor = uiColor;
        }
    });
}


- (void)setBackgroundColor:(WidgetId)widgetId color:(Color)color
{
    auto widget = [self guiFindWidget:widgetId];
    widget.backgroundColor = toUIColor(color);
}

- (void)setText:(WidgetId)widgetId text:(NSString *)text
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto ctx = [self findContext:widgetId];
        if (ctx->type == WidgetType::Input) {
            auto textField = objc_cast<UITextField>(ctx->widget);
            MDebugTrapAssert(textField);
            textField.text = text;
        } else if (ctx->type == WidgetType::Label) {
            auto label = objc_cast<UILabel>(ctx->widget);
            MDebugTrapAssert(label);
            label.text = text;
        }
        
    });
}

- (NSString *)getText:(WidgetId)widgetId
{
    // TODO: implement multithreading
    auto ctx = [self findContext:widgetId];

    if (ctx->type == WidgetType::Input) {
        auto textField = objc_cast<UITextField>(ctx->widget);
        MDebugTrapAssert(textField);
        return textField.text;
    }

    if (ctx->type == WidgetType::Label) {
        auto label = objc_cast<UILabel>(ctx->widget);
        MDebugTrapAssert(label);
        return label.text;
    }

    return nil;
}

- (void)setVisible:(WidgetId)widgetId visible:(bool)visible {
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto widget = [self guiFindWidget:widgetId];
        widget.hidden = !visible;
        [widget setNeedsDisplay];
        
    });
}

- (void)moveWidget:(WidgetId)widgetId frame:(RectA)frame {
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto widget = [self guiFindWidget:widgetId];
        widget.frame = toCGRect(Env::toScreen(frame));
        [widget setNeedsDisplay];
        
    });
}

- (void)moveWidget:(WidgetId)widgetId point:(mangl::Point)point
{
    // TODO:
}


- (void)setFont:(WidgetId)widgetId size:(Cord)size fontName:(NSString *)fontName {
    dispatch_async(dispatch_get_main_queue(), ^{
        
        auto font = [UIFont systemFontOfSize:size * Env::toScreenCoord(size)];
        auto ctx = [self findContext:widgetId];
        if (ctx->type == WidgetType::Input) {
            auto textField = objc_cast<UITextField>(ctx->widget);
            MDebugTrapAssert(textField);
            [textField setFont:font];
        } else if (ctx->type == WidgetType::Label) {
            auto label = objc_cast<UILabel>(ctx->widget);
            MDebugTrapAssert(label);
            [label setFont:font];
        }
        
    });
}


//
// MARK: UITextFieldDelegate
//
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    
    [textField resignFirstResponder];
    return YES;
}

@end

//
// MARK: MNativeGui
//
namespace mangl {

void NativeGui::alert(AlertParamCR parm)
{
    [ManglGuiHelper.instance alert:toNSString(parm.message) withTitle:toNSString(parm.title)];
}

auto NativeGui::clearWidgets() -> SelfRet
{
    [ManglGuiHelper.instance clearWidgets];
    return this;
}

WidgetId NativeGui::inputDialog(InputDialogParamA params)
{
    return [ManglGuiHelper.instance inputDialog:params];
}

WidgetId NativeGui::createInput(StrView text, RectA frame, Flags flags)
{
    return [ManglGuiHelper.instance createInput:toNSString(text) frame:frame flags:flags];
}

WidgetId NativeGui::createLabel(StrView text, RectA frame, Flags flags)
{
    return [ManglGuiHelper.instance createLabel:toNSString(text) frame:frame flags:flags];
}

void NativeGui::setInputPlaceholder(WidgetId widgetId, StrView text)
{
    [ManglGuiHelper.instance setInputPlaceholder:widgetId text:toNSString(text)];
}

auto NativeGui::setText(WidgetId widgetId, StrView text) -> SelfRet
{
    [ManglGuiHelper.instance setText:widgetId text:toNSString(text)];
    return this;
}

String NativeGui::getText(WidgetId widgetId)
{
    return [ManglGuiHelper.instance getText:widgetId].UTF8String;
}


auto NativeGui::setVisible(WidgetId widgetId, bool visible) -> SelfRet
{
    [ManglGuiHelper.instance setVisible:widgetId visible:visible];
    return this;
}

auto NativeGui::move(WidgetId widgetId, RectA frame) -> SelfRet
{
    [ManglGuiHelper.instance moveWidget:widgetId frame:frame];
    return this;
}

auto NativeGui::move(WidgetId widgetId, Point pos) -> SelfRet
{
    [ManglGuiHelper.instance moveWidget:widgetId point:pos];
    return this;
}


auto NativeGui::setTextColor(WidgetId widgetId, ColorA color) -> SelfRet
{
    [ManglGuiHelper.instance setTextColor:widgetId color:color];
    return this;
}

auto NativeGui::setBackgroundColor(WidgetId widgetId, ColorA color) -> SelfRet
{
    [ManglGuiHelper.instance setBackgroundColor:widgetId color:color];
    return this;
}

auto NativeGui::setFont(WidgetId widgetId, Cord size, StrView fontName) -> SelfRet
{
    auto fontSize = Env::toScreenCoord(size);
    auto nsFontName = fontName.empty() ? nil : toNSString(fontName);
    [ManglGuiHelper.instance setFont:widgetId size:fontSize fontName:nsFontName];
    return this;
}

}   // namespace mangl
