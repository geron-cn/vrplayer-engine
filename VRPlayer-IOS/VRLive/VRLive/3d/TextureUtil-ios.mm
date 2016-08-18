#import<CoreFoundation/CoreFoundation.h>
#import <CoreText/CoreText.h>
#import <UIKit/UIKit.h>

#include "StringTextureUtil.h"
#include <assert.h>
#include "FileUtils/Data.h"

namespace FontUtils {
    NSMutableParagraphStyle* _calculateParagraphStyle(bool enableWrap, int overflow)
    {
        NSMutableParagraphStyle* paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
        return paragraphStyle;
    }
    
    NSTextAlignment _calculateTextAlignment(vrlive::TextAlign align)
    {
        unsigned uHoriFlag = (int)align & 0x0f;
        NSTextAlignment nsAlign = (2 == uHoriFlag) ? NSTextAlignmentRight
        : (3 == uHoriFlag) ? NSTextAlignmentCenter
        : NSTextAlignmentLeft;
        
        return nsAlign;
    }
    
    
    
    CGFloat _calculateTextDrawStartWidth(vrlive::TextAlign align, CGSize realDimensions, CGSize dimensions)
    {
        CGFloat xPadding = 0;
        unsigned uHoriFlag = (int)align & 0x0f;
        switch (uHoriFlag) {
                //center
            case 3: xPadding = (dimensions.width - realDimensions.width) / 2.0f; break;
                //right
            case 2: xPadding = dimensions.width - realDimensions.width; break;
            default: break;
        }
        return xPadding;
    }
    
}

namespace vrlive {
    
    typedef struct
    {
        unsigned int height;
        unsigned int width;
        bool         isPremultipliedAlpha;
        bool         hasShadow;
        CGSize       shadowOffset;
        float        shadowBlur;
        float        shadowOpacity;
        bool         hasStroke;
        float        strokeColorR;
        float        strokeColorG;
        float        strokeColorB;
        float        strokeColorA;
        float        strokeSize;
        float        tintColorR;
        float        tintColorG;
        float        tintColorB;
        float        tintColorA;
        
        unsigned char*  data;
        
    } tImageInfo;
    
    static NSAttributedString* __attributedStringWithFontSize(NSMutableAttributedString* attributedString, CGFloat fontSize)
    {
        {
            [attributedString beginEditing];
            
            [attributedString enumerateAttribute:NSFontAttributeName inRange:NSMakeRange(0, attributedString.length) options:0 usingBlock:^(id value, NSRange range, BOOL *stop) {
                
                UIFont* font = value;
                font = [font fontWithSize:fontSize];
                
                [attributedString removeAttribute:NSFontAttributeName range:range];
                [attributedString addAttribute:NSFontAttributeName value:font range:range];
            }];
            
            [attributedString endEditing];
        }
        
        return [[attributedString copy] autorelease];
    }
    
    static CGFloat _calculateTextDrawStartHeight(TextAlign align, CGSize realDimensions, CGSize dimensions)
    {
        float startH = 0;
        // vertical alignment
        unsigned int vAlignment = ((int)align >> 4) & 0x0F;
        switch (vAlignment) {
                //bottom
            case 2:startH = dimensions.height - realDimensions.height;break;
                //top
            case 1:startH = 0;break;
                //center
            case 3: startH = (dimensions.height - realDimensions.height) / 2;break;
            default:
                break;
        }
        return startH;
    }
    
    static CGSize _calculateShrinkedSizeForString(NSAttributedString **str, id font, CGSize constrainSize, bool enableWrap, int& newFontSize)
    {
        CGRect actualSize = CGRectMake(0, 0, constrainSize.width + 1, constrainSize.height + 1);
        int fontSize = [font pointSize];
        fontSize = fontSize + 1;
        
        if (!enableWrap) {
            while (actualSize.size.width > constrainSize.width ||
                   actualSize.size.height > constrainSize.height) {
                fontSize = fontSize - 1;
                
                NSMutableAttributedString *mutableString = [[*str mutableCopy] autorelease];
                *str = __attributedStringWithFontSize(mutableString, fontSize);
                
                CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)*str);
                CGSize targetSize = CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX);
                CGSize fitSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, [(*str) length]), NULL, targetSize, NULL);
                CFRelease(framesetter);
                if (fitSize.width == 0 || fitSize.height == 0) {
                    continue;
                }
                
                actualSize.size = fitSize;
                
                if (constrainSize.width <= 0) {
                    constrainSize.width = fitSize.width;
                }
                if (constrainSize.height <= 0) {
                    constrainSize.height = fitSize.height;
                }
                if (fontSize <= 0) {
                    break;
                }
            }
            
        }
        else {
            while (actualSize.size.height > constrainSize.height ||
                   actualSize.size.width > constrainSize.width) {
                fontSize = fontSize - 1;
                
                NSMutableAttributedString *mutableString = [[*str mutableCopy] autorelease];
                *str = __attributedStringWithFontSize(mutableString, fontSize);
                
                CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)*str);
                CGSize targetSize = CGSizeMake(constrainSize.width, CGFLOAT_MAX);
                CGSize fitSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, [(*str) length]), NULL, targetSize, NULL);
                CFRelease(framesetter);
                if (fitSize.width == 0 || fitSize.height == 0) {
                    continue;
                }
                
                actualSize.size = fitSize;
                
                if (constrainSize.height <= 0) {
                    constrainSize.height = fitSize.height;
                }
                if (constrainSize.width <= 0) {
                    constrainSize.width = fitSize.width;
                }
                if (fontSize <= 0) {
                    break;
                }
            }
            
        }
        
        newFontSize = fontSize;
        
        return CGSizeMake(actualSize.size.width, actualSize.size.height);
    }

    
    static CGSize _calculateStringSize(NSAttributedString *str, id font, CGSize *constrainSize, bool enableWrap, int overflow)
    {
        CGSize textRect = CGSizeZero;
        textRect.width = constrainSize->width > 0 ? constrainSize->width
        : CGFLOAT_MAX;
        textRect.height = constrainSize->height > 0 ? constrainSize->height
        : CGFLOAT_MAX;
        
        if (overflow == 1) {
            if(!enableWrap) {
                textRect.width = CGFLOAT_MAX;
                textRect.height = CGFLOAT_MAX;
            } else {
                textRect.height = CGFLOAT_MAX;
            }
        }
        
        CGSize dim;
        dim = [str boundingRectWithSize:CGSizeMake(textRect.width, textRect.height)
                                options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading)
                                context:nil].size;
        
        dim.width = ceilf(dim.width);
        dim.height = ceilf(dim.height);
        
        return dim;
    }
    
    static id _createSystemFont( const char * fontName, int size)
    {
        NSString * fntName      = [NSString stringWithUTF8String:fontName];
        // On iOS custom fonts must be listed beforehand in the App info.plist (in order to be usable) and referenced only the by the font family name itself when
        // calling [UIFont fontWithName]. Therefore even if the developer adds 'SomeFont.ttf' or 'fonts/SomeFont.ttf' to the App .plist, the font must
        // be referenced as 'SomeFont' when calling [UIFont fontWithName]. Hence we strip out the folder path components and the extension here in order to get just
        // the font family name itself. This stripping step is required especially for references to user fonts stored in CCB files; CCB files appear to store
        // the '.ttf' extensions when referring to custom fonts.
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
        
        // create the font
        id font = [UIFont fontWithName:fntName size:size];
        
        if (!font)
        {
            font = [UIFont systemFontOfSize:size];
        }
        return font;
    }

    static bool _initWithString(const char * text, TextAlign align, const char * fontName, int size, tImageInfo* info, bool enableWrap, int overflow)
    {
        
        bool bRet = false;
        do
        {
            if(! text || ! info) break;
            
            id font = _createSystemFont(fontName, size);
            
            if (! font) break;
            
            NSString * str          = [NSString stringWithUTF8String:text];
            if(!str) break;
            
            CGSize dimensions;
            dimensions.width     = info->width;
            dimensions.height    = info->height;
            
            NSTextAlignment nsAlign = FontUtils::_calculateTextAlignment(align);
            NSMutableParagraphStyle* paragraphStyle = FontUtils::_calculateParagraphStyle(enableWrap, overflow);
            paragraphStyle.alignment = nsAlign;
            
            // measure text size with specified font and determine the rectangle to draw text in
            
            UIColor *foregroundColor = [UIColor colorWithRed:info->tintColorR
                                                       green:info->tintColorG
                                                        blue:info->tintColorB
                                                       alpha:info->tintColorA];
            
            // adjust text rect according to overflow
            NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                        foregroundColor,NSForegroundColorAttributeName,
                                                        font, NSFontAttributeName,
                                                        paragraphStyle, NSParagraphStyleAttributeName, nil];
            
            NSAttributedString *stringWithAttributes =[[[NSAttributedString alloc] initWithString:str
                                                                                       attributes:tokenAttributesDict] autorelease];
            
            int shrinkFontSize = size;
            CGSize realDimensions;
            if (overflow == 2) {
                realDimensions = _calculateShrinkedSizeForString(&stringWithAttributes, font, dimensions, enableWrap, shrinkFontSize);
            } else {
                realDimensions = _calculateStringSize(stringWithAttributes, font, &dimensions, enableWrap, overflow);
            }
            
            
            if(realDimensions.width <= 0 || realDimensions.height <= 0) break;
            if (dimensions.width <= 0) {
                dimensions.width = realDimensions.width;
            }
            if (dimensions.height <= 0) {
                dimensions.height = realDimensions.height;
            }
            
            // compute start point
            CGFloat yPadding = _calculateTextDrawStartHeight(align, realDimensions, dimensions);
            CGFloat xPadding = FontUtils::_calculateTextDrawStartWidth(align, realDimensions, dimensions);
            
            NSInteger POTWide = dimensions.width;
            NSInteger POTHigh = dimensions.height;
            
            CGRect textRect = CGRectMake(xPadding, yPadding,
                                         realDimensions.width, realDimensions.height);
            
            
            NSUInteger textureSize = POTWide * POTHigh * 4;
            unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * textureSize);
            memset(data, 0, textureSize);
            
            // draw text
            CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
            CGContextRef context        = CGBitmapContextCreate(data,
                                                                POTWide,
                                                                POTHigh,
                                                                8,
                                                                POTWide * 4,
                                                                colorSpace,
                                                                kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
            if (!context)
            {
                CGColorSpaceRelease(colorSpace);
                if(data)break;
                break;
            }
            
            // text color
            CGContextSetRGBFillColor(context,
                                     info->tintColorR,
                                     info->tintColorG,
                                     info->tintColorB,
                                     info->tintColorA);
            
            // move Y rendering to the top of the image
            CGContextTranslateCTM(context, 0.0f, POTHigh);
            
            //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
            CGContextScaleCTM(context, 1.0f, -1.0f);
            // store the current context
            UIGraphicsPushContext(context);
            
            CGColorSpaceRelease(colorSpace);
            
            CGContextSetShouldSubpixelQuantizeFonts(context, false);
            
            CGContextBeginTransparencyLayerWithRect(context, textRect, NULL);
            
            
            if ( info->hasStroke )
            {
                CGContextSetTextDrawingMode(context, kCGTextStroke);
                UIColor *strokeColor = [UIColor colorWithRed:info->strokeColorR
                                                       green:info->strokeColorG
                                                        blue:info->strokeColorB
                                                       alpha:info->strokeColorA];
                
                
                NSMutableDictionary* tokenAttributesDict2 = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                             foregroundColor,NSForegroundColorAttributeName,
                                                             font, NSFontAttributeName,
                                                             paragraphStyle, NSParagraphStyleAttributeName, nil];
                [tokenAttributesDict2 setObject:[NSNumber numberWithFloat: info->strokeSize / shrinkFontSize * 100]
                                         forKey:NSStrokeWidthAttributeName];
                [tokenAttributesDict2 setObject:strokeColor forKey:NSStrokeColorAttributeName];
                
                NSAttributedString *strokeString =[[[NSAttributedString alloc] initWithString:str
                                                                                   attributes:tokenAttributesDict2] autorelease];
                
                if(overflow == 2){
                    _calculateShrinkedSizeForString(&strokeString, font, dimensions, enableWrap, shrinkFontSize);
                }
                
                
                [strokeString drawInRect:textRect];
                
            }
            
            CGContextSetTextDrawingMode(context, kCGTextFill);
            
            // actually draw the text in the context
            [stringWithAttributes drawInRect:textRect];
            
            CGContextEndTransparencyLayer(context);
            
            // pop the context
            UIGraphicsPopContext();
            
            // release the context
            CGContextRelease(context);
            
            // output params
            info->data                 = data;
            info->isPremultipliedAlpha = true;
            info->width                = static_cast<int>(POTWide);
            info->height               = static_cast<int>(POTHigh);
            bRet                        = true;
            
        } while (0);
        
        return bRet;
    }
    
    float StringTextureUtil::getScaleFactor()
    {
        return [[UIScreen mainScreen] scale];
    }
    
    vrlive::Data StringTextureUtil::getTextureDataForText(const char * text, const FontDefinition& textDefinition, TextAlign align, int &width, int &height, bool& hasPremultipliedAlpha)
    {
        vrlive::Data ret;
        
        do {
            tImageInfo info = {0};
            info.width                  = textDefinition._dimensions[0];
            info.height                 = textDefinition._dimensions[1];
            info.hasShadow              = false;
            info.shadowOffset.width     = 0;
            info.shadowOffset.height    = 0;
            info.shadowBlur             = 0;
            info.shadowOpacity          = 0;
            info.hasStroke              = false;
            info.strokeColorR           = 0;
            info.strokeColorG           = 0;
            info.strokeColorB           = 0;
            info.strokeColorA           = 0;
            info.strokeSize             = 0;
            info.tintColorR             = textDefinition._fontFillColor.x;
            info.tintColorG             = textDefinition._fontFillColor.y;
            info.tintColorB             = textDefinition._fontFillColor.z;
            info.tintColorA             = textDefinition._fontAlpha / 255.0f;
            
            if (! _initWithString(text, align, textDefinition._fontName.c_str(), textDefinition._fontSize, &info, textDefinition._enableWrap, textDefinition._overflow))
            {
                break;
            }
            height = info.height;
            width = info.width;
            ret.fastSet(info.data,width * height * 4);
            hasPremultipliedAlpha = true;
        } while (0);
        
        return ret;
    }
}
