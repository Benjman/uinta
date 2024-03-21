#ifndef SRC_ENGINE_INCLUDE_UINTA_COLOR_H_
#define SRC_ENGINE_INCLUDE_UINTA_COLOR_H_

#include <string>

#include "glm/ext/vector_float4.hpp"

namespace uinta {
namespace color {

using Color = glm::vec4;

constexpr Color Red50(1, 0.921569, 0.933333, 1);
constexpr Color Red100(1, 0.803922, 0.823529, 1);
constexpr Color Red200(0.937255, 0.603922, 0.603922, 1);
constexpr Color Red300(0.898039, 0.45098, 0.45098, 1);
constexpr Color Red400(0.937255, 0.32549, 0.313725, 1);
constexpr Color Red500(0.956863, 0.262745, 0.211765, 1);
constexpr Color Red600(0.898039, 0.223529, 0.207843, 1);
constexpr Color Red700(0.827451, 0.184314, 0.184314, 1);
constexpr Color Red800(0.776471, 0.156863, 0.156863, 1);
constexpr Color Red900(0.717647, 0.109804, 0.109804, 1);
constexpr Color RedA100(1, 0.541176, 0.501961, 1);
constexpr Color RedA200(1, 0.321569, 0.321569, 1);
constexpr Color RedA400(1, 0.090196, 0.266667, 1);
constexpr Color RedA700(0.835294, 0, 0, 1);

constexpr Color Pink50(0.988235, 0.894118, 0.92549, 1);
constexpr Color Pink100(0.972549, 0.733333, 0.815686, 1);
constexpr Color Pink200(0.956863, 0.560784, 0.694118, 1);
constexpr Color Pink300(0.941176, 0.384314, 0.572549, 1);
constexpr Color Pink400(0.92549, 0.25098, 0.478431, 1);
constexpr Color Pink500(0.913725, 0.117647, 0.388235, 1);
constexpr Color Pink600(0.847059, 0.105882, 0.376471, 1);
constexpr Color Pink700(0.760784, 0.094118, 0.356863, 1);
constexpr Color Pink800(0.678431, 0.078431, 0.341176, 1);
constexpr Color Pink900(0.533333, 0.054902, 0.309804, 1);
constexpr Color PinkA100(1, 0.501961, 0.670588, 1);
constexpr Color PinkA200(1, 0.25098, 0.505882, 1);
constexpr Color PinkA400(0.960784, 0, 0.341176, 1);
constexpr Color PinkA700(0.772549, 0.066667, 0.384314, 1);

constexpr Color Purple50(0.952941, 0.898039, 0.960784, 1);
constexpr Color Purple100(0.882353, 0.745098, 0.905882, 1);
constexpr Color Purple200(0.807843, 0.576471, 0.847059, 1);
constexpr Color Purple300(0.729412, 0.407843, 0.784314, 1);
constexpr Color Purple400(0.670588, 0.278431, 0.737255, 1);
constexpr Color Purple500(0.611765, 0.152941, 0.690196, 1);
constexpr Color Purple600(0.556863, 0.141176, 0.666667, 1);
constexpr Color Purple700(0.482353, 0.121569, 0.635294, 1);
constexpr Color Purple800(0.415686, 0.105882, 0.603922, 1);
constexpr Color Purple900(0.290196, 0.078431, 0.54902, 1);
constexpr Color PurpleA100(0.917647, 0.501961, 0.988235, 1);
constexpr Color PurpleA200(0.878431, 0.25098, 0.984314, 1);
constexpr Color PurpleA400(0.835294, 0, 0.976471, 1);
constexpr Color PurpleA700(0.666667, 0, 1, 1);

constexpr Color DeepPurple50(0.929412, 0.905882, 0.964706, 1);
constexpr Color DeepPurple100(0.819608, 0.768627, 0.913725, 1);
constexpr Color DeepPurple200(0.701961, 0.615686, 0.858824, 1);
constexpr Color DeepPurple300(0.584314, 0.458824, 0.803922, 1);
constexpr Color DeepPurple400(0.494118, 0.341176, 0.760784, 1);
constexpr Color DeepPurple500(0.403922, 0.227451, 0.717647, 1);
constexpr Color DeepPurple600(0.368627, 0.207843, 0.694118, 1);
constexpr Color DeepPurple700(0.317647, 0.176471, 0.658824, 1);
constexpr Color DeepPurple800(0.270588, 0.152941, 0.627451, 1);
constexpr Color DeepPurple900(0.192157, 0.105882, 0.572549, 1);
constexpr Color DeepPurpleA100(0.701961, 0.533333, 1, 1);
constexpr Color DeepPurpleA200(0.486275, 0.301961, 1, 1);
constexpr Color DeepPurpleA400(0.396078, 0.121569, 1, 1);
constexpr Color DeepPurpleA700(0.384314, 0, 0.917647, 1);

constexpr Color Indigo50(0.909804, 0.917647, 0.964706, 1);
constexpr Color Indigo100(0.772549, 0.792157, 0.913725, 1);
constexpr Color Indigo200(0.623529, 0.658824, 0.854902, 1);
constexpr Color Indigo300(0.47451, 0.52549, 0.796078, 1);
constexpr Color Indigo400(0.360784, 0.419608, 0.752941, 1);
constexpr Color Indigo500(0.247059, 0.317647, 0.709804, 1);
constexpr Color Indigo600(0.223529, 0.286275, 0.670588, 1);
constexpr Color Indigo700(0.188235, 0.247059, 0.623529, 1);
constexpr Color Indigo800(0.156863, 0.207843, 0.576471, 1);
constexpr Color Indigo900(0.101961, 0.137255, 0.494118, 1);
constexpr Color IndigoA100(0.54902, 0.619608, 1, 1);
constexpr Color IndigoA200(0.32549, 0.427451, 0.996078, 1);
constexpr Color IndigoA400(0.239216, 0.352941, 0.996078, 1);
constexpr Color IndigoA700(0.188235, 0.309804, 0.996078, 1);

constexpr Color Blue50(0.890196, 0.94902, 0.992157, 1);
constexpr Color Blue100(0.733333, 0.870588, 0.984314, 1);
constexpr Color Blue200(0.564706, 0.792157, 0.976471, 1);
constexpr Color Blue300(0.392157, 0.709804, 0.964706, 1);
constexpr Color Blue400(0.258824, 0.647059, 0.960784, 1);
constexpr Color Blue500(0.129412, 0.588235, 0.952941, 1);
constexpr Color Blue600(0.117647, 0.533333, 0.898039, 1);
constexpr Color Blue700(0.098039, 0.462745, 0.823529, 1);
constexpr Color Blue800(0.082353, 0.396078, 0.752941, 1);
constexpr Color Blue900(0.05098, 0.278431, 0.631373, 1);
constexpr Color BlueA100(0.509804, 0.694118, 1, 1);
constexpr Color BlueA200(0.266667, 0.541176, 1, 1);
constexpr Color BlueA400(0.160784, 0.47451, 1, 1);
constexpr Color BlueA700(0.160784, 0.384314, 1, 1);

constexpr Color LightBlue50(0.882353, 0.960784, 0.996078, 1);
constexpr Color LightBlue100(0.701961, 0.898039, 0.988235, 1);
constexpr Color LightBlue200(0.505882, 0.831373, 0.980392, 1);
constexpr Color LightBlue300(0.309804, 0.764706, 0.968627, 1);
constexpr Color LightBlue400(0.160784, 0.713725, 0.964706, 1);
constexpr Color LightBlue500(0.011765, 0.662745, 0.956863, 1);
constexpr Color LightBlue600(0.011765, 0.607843, 0.898039, 1);
constexpr Color LightBlue700(0.007843, 0.533333, 0.819608, 1);
constexpr Color LightBlue800(0.007843, 0.466667, 0.741176, 1);
constexpr Color LightBlue900(0.003922, 0.341176, 0.607843, 1);
constexpr Color LightBlueA100(0.501961, 0.847059, 1, 1);
constexpr Color LightBlueA200(0.25098, 0.768627, 1, 1);
constexpr Color LightBlueA400(0, 0.690196, 1, 1);
constexpr Color LightBlueA700(0, 0.568627, 0.917647, 1);

constexpr Color Cyan50(0.878431, 0.968627, 0.980392, 1);
constexpr Color Cyan100(0.698039, 0.921569, 0.94902, 1);
constexpr Color Cyan200(0.501961, 0.870588, 0.917647, 1);
constexpr Color Cyan300(0.301961, 0.815686, 0.882353, 1);
constexpr Color Cyan400(0.14902, 0.776471, 0.854902, 1);
constexpr Color Cyan500(0, 0.737255, 0.831373, 1);
constexpr Color Cyan600(0, 0.67451, 0.756863, 1);
constexpr Color Cyan700(0, 0.592157, 0.654902, 1);
constexpr Color Cyan800(0, 0.513725, 0.560784, 1);
constexpr Color Cyan900(0, 0.376471, 0.392157, 1);
constexpr Color CyanA100(0.517647, 1, 1, 1);
constexpr Color CyanA200(0.094118, 1, 1, 1);
constexpr Color CyanA400(0, 0.898039, 1, 1);
constexpr Color CyanA700(0, 0.721569, 0.831373, 1);

constexpr Color Teal50(0.878431, 0.94902, 0.945098, 1);
constexpr Color Teal100(0.698039, 0.87451, 0.858824, 1);
constexpr Color Teal200(0.501961, 0.796078, 0.768627, 1);
constexpr Color Teal300(0.301961, 0.713725, 0.67451, 1);
constexpr Color Teal400(0.14902, 0.65098, 0.603922, 1);
constexpr Color Teal500(0, 0.588235, 0.533333, 1);
constexpr Color Teal600(0, 0.537255, 0.482353, 1);
constexpr Color Teal700(0, 0.47451, 0.419608, 1);
constexpr Color Teal800(0, 0.411765, 0.360784, 1);
constexpr Color Teal900(0, 0.301961, 0.25098, 1);
constexpr Color TealA100(0.654902, 1, 0.921569, 1);
constexpr Color TealA200(0.392157, 1, 0.854902, 1);
constexpr Color TealA400(0.113725, 0.913725, 0.713725, 1);
constexpr Color TealA700(0, 0.74902, 0.647059, 1);

constexpr Color Green50(0.909804, 0.960784, 0.913725, 1);
constexpr Color Green100(0.784314, 0.901961, 0.788235, 1);
constexpr Color Green200(0.647059, 0.839216, 0.654902, 1);
constexpr Color Green300(0.505882, 0.780392, 0.517647, 1);
constexpr Color Green400(0.4, 0.733333, 0.415686, 1);
constexpr Color Green500(0.298039, 0.686275, 0.313725, 1);
constexpr Color Green600(0.262745, 0.627451, 0.278431, 1);
constexpr Color Green700(0.219608, 0.556863, 0.235294, 1);
constexpr Color Green800(0.180392, 0.490196, 0.196078, 1);
constexpr Color Green900(0.105882, 0.368627, 0.12549, 1);
constexpr Color GreenA100(0.72549, 0.964706, 0.792157, 1);
constexpr Color GreenA200(0.411765, 0.941176, 0.682353, 1);
constexpr Color GreenA400(0, 0.901961, 0.462745, 1);
constexpr Color GreenA700(0, 0.784314, 0.32549, 1);

constexpr Color LightGreen50(0.945098, 0.972549, 0.913725, 1);
constexpr Color LightGreen100(0.862745, 0.929412, 0.784314, 1);
constexpr Color LightGreen200(0.772549, 0.882353, 0.647059, 1);
constexpr Color LightGreen300(0.682353, 0.835294, 0.505882, 1);
constexpr Color LightGreen400(0.611765, 0.8, 0.396078, 1);
constexpr Color LightGreen500(0.545098, 0.764706, 0.290196, 1);
constexpr Color LightGreen600(0.486275, 0.701961, 0.258824, 1);
constexpr Color LightGreen700(0.407843, 0.623529, 0.219608, 1);
constexpr Color LightGreen800(0.333333, 0.545098, 0.184314, 1);
constexpr Color LightGreen900(0.2, 0.411765, 0.117647, 1);
constexpr Color LightGreenA100(0.8, 1, 0.564706, 1);
constexpr Color LightGreenA200(0.698039, 1, 0.34902, 1);
constexpr Color LightGreenA400(0.462745, 1, 0.011765, 1);
constexpr Color LightGreenA700(0.392157, 0.866667, 0.090196, 1);

constexpr Color Lime50(0.976471, 0.984314, 0.905882, 1);
constexpr Color Lime100(0.941176, 0.956863, 0.764706, 1);
constexpr Color Lime200(0.901961, 0.933333, 0.611765, 1);
constexpr Color Lime300(0.862745, 0.905882, 0.458824, 1);
constexpr Color Lime400(0.831373, 0.882353, 0.341176, 1);
constexpr Color Lime500(0.803922, 0.862745, 0.223529, 1);
constexpr Color Lime600(0.752941, 0.792157, 0.2, 1);
constexpr Color Lime700(0.686275, 0.705882, 0.168627, 1);
constexpr Color Lime800(0.619608, 0.615686, 0.141176, 1);
constexpr Color Lime900(0.509804, 0.466667, 0.090196, 1);
constexpr Color LimeA100(0.956863, 1, 0.505882, 1);
constexpr Color LimeA200(0.933333, 1, 0.254902, 1);
constexpr Color LimeA400(0.776471, 1, 0, 1);
constexpr Color LimeA700(0.682353, 0.917647, 0, 1);

constexpr Color Yellow50(1, 0.992157, 0.905882, 1);
constexpr Color Yellow100(1, 0.976471, 0.768627, 1);
constexpr Color Yellow200(1, 0.960784, 0.615686, 1);
constexpr Color Yellow300(1, 0.945098, 0.462745, 1);
constexpr Color Yellow400(1, 0.933333, 0.345098, 1);
constexpr Color Yellow500(1, 0.921569, 0.231373, 1);
constexpr Color Yellow600(0.992157, 0.847059, 0.207843, 1);
constexpr Color Yellow700(0.984314, 0.752941, 0.176471, 1);
constexpr Color Yellow800(0.976471, 0.658824, 0.145098, 1);
constexpr Color Yellow900(0.960784, 0.498039, 0.090196, 1);
constexpr Color YellowA100(1, 1, 0.552941, 1);
constexpr Color YellowA200(1, 1, 0, 1);
constexpr Color YellowA400(1, 0.917647, 0, 1);
constexpr Color YellowA700(1, 0.839216, 0, 1);

constexpr Color Amber50(1, 0.972549, 0.882353, 1);
constexpr Color Amber100(1, 0.92549, 0.701961, 1);
constexpr Color Amber200(1, 0.878431, 0.509804, 1);
constexpr Color Amber300(1, 0.835294, 0.309804, 1);
constexpr Color Amber400(1, 0.792157, 0.156863, 1);
constexpr Color Amber500(1, 0.756863, 0.027451, 1);
constexpr Color Amber600(1, 0.701961, 0, 1);
constexpr Color Amber700(1, 0.627451, 0, 1);
constexpr Color Amber800(1, 0.560784, 0, 1);
constexpr Color Amber900(1, 0.435294, 0, 1);
constexpr Color AmberA100(1, 0.898039, 0.498039, 1);
constexpr Color AmberA200(1, 0.843137, 0.25098, 1);
constexpr Color AmberA400(1, 0.768627, 0, 1);
constexpr Color AmberA700(1, 0.670588, 0, 1);

constexpr Color Orange50(1, 0.952941, 0.878431, 1);
constexpr Color Orange100(1, 0.878431, 0.698039, 1);
constexpr Color Orange200(1, 0.8, 0.501961, 1);
constexpr Color Orange300(1, 0.717647, 0.301961, 1);
constexpr Color Orange400(1, 0.654902, 0.14902, 1);
constexpr Color Orange500(1, 0.596078, 0, 1);
constexpr Color Orange600(0.984314, 0.54902, 0, 1);
constexpr Color Orange700(0.960784, 0.486275, 0, 1);
constexpr Color Orange800(0.937255, 0.423529, 0, 1);
constexpr Color Orange900(0.901961, 0.317647, 0, 1);
constexpr Color OrangeA100(1, 0.819608, 0.501961, 1);
constexpr Color OrangeA200(1, 0.670588, 0.25098, 1);
constexpr Color OrangeA400(1, 0.568627, 0, 1);
constexpr Color OrangeA700(1, 0.427451, 0, 1);

constexpr Color DeepOrange50(0.984314, 0.913725, 0.905882, 1);
constexpr Color DeepOrange100(1, 0.8, 0.737255, 1);
constexpr Color DeepOrange200(1, 0.670588, 0.568627, 1);
constexpr Color DeepOrange300(1, 0.541176, 0.396078, 1);
constexpr Color DeepOrange400(1, 0.439216, 0.262745, 1);
constexpr Color DeepOrange500(1, 0.341176, 0.133333, 1);
constexpr Color DeepOrange600(0.956863, 0.317647, 0.117647, 1);
constexpr Color DeepOrange700(0.901961, 0.290196, 0.098039, 1);
constexpr Color DeepOrange800(0.847059, 0.262745, 0.082353, 1);
constexpr Color DeepOrange900(0.74902, 0.211765, 0.047059, 1);
constexpr Color DeepOrangeA100(1, 0.619608, 0.501961, 1);
constexpr Color DeepOrangeA200(1, 0.431373, 0.25098, 1);
constexpr Color DeepOrangeA400(1, 0.239216, 0, 1);
constexpr Color DeepOrangeA700(0.866667, 0.172549, 0, 1);

constexpr Color Brown50(0.937255, 0.921569, 0.913725, 1);
constexpr Color Brown100(0.843137, 0.8, 0.784314, 1);
constexpr Color Brown200(0.737255, 0.666667, 0.643137, 1);
constexpr Color Brown300(0.631373, 0.533333, 0.498039, 1);
constexpr Color Brown400(0.552941, 0.431373, 0.388235, 1);
constexpr Color Brown500(0.47451, 0.333333, 0.282353, 1);
constexpr Color Brown600(0.427451, 0.298039, 0.254902, 1);
constexpr Color Brown700(0.364706, 0.25098, 0.215686, 1);
constexpr Color Brown800(0.305882, 0.203922, 0.180392, 1);
constexpr Color Brown900(0.243137, 0.152941, 0.137255, 1);

constexpr Color Gray50(0.980392, 0.980392, 0.980392, 1);
constexpr Color Gray100(0.960784, 0.960784, 0.960784, 1);
constexpr Color Gray200(0.933333, 0.933333, 0.933333, 1);
constexpr Color Gray300(0.878431, 0.878431, 0.878431, 1);
constexpr Color Gray400(0.741176, 0.741176, 0.741176, 1);
constexpr Color Gray500(0.619608, 0.619608, 0.619608, 1);
constexpr Color Gray600(0.458824, 0.458824, 0.458824, 1);
constexpr Color Gray700(0.380392, 0.380392, 0.380392, 1);
constexpr Color Gray800(0.258824, 0.258824, 0.258824, 1);
constexpr Color Gray900(0.129412, 0.129412, 0.129412, 1);

constexpr Color BlueGray50(0.92549, 0.937255, 0.945098, 1);
constexpr Color BlueGray100(0.811765, 0.847059, 0.862745, 1);
constexpr Color BlueGray200(0.690196, 0.745098, 0.772549, 1);
constexpr Color BlueGray300(0.564706, 0.643137, 0.682353, 1);
constexpr Color BlueGray400(0.470588, 0.564706, 0.611765, 1);
constexpr Color BlueGray500(0.376471, 0.490196, 0.545098, 1);
constexpr Color BlueGray600(0.329412, 0.431373, 0.478431, 1);
constexpr Color BlueGray700(0.270588, 0.352941, 0.392157, 1);
constexpr Color BlueGray800(0.215686, 0.278431, 0.309804, 1);
constexpr Color BlueGray900(0.14902, 0.196078, 0.219608, 1);

constexpr Color Black(0, 0, 0, 1);
constexpr Color White(1, 1, 1, 1);

Color RandomColor() noexcept;

std::string to_string(Color) noexcept;

}  // namespace color

using Color = color::Color;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_COLOR_H_
