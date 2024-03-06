//
// Created by Daniel Becher on 2/24/24.
//

#include "UIElement.h"

#include "Font.h"
#include "../state/GlobalState.h"
#include "../SpriteBatch.h"

// TODO: separate width/height radii
// TODO: adjust for when curve = 0

/*
 *  110, 24, 502, 48
 *  37, 355, 119, 101
 *  157, 355, 173, 101
 *  331, 355, 353, 101
 */

UIElement::UIElement(UIElementBuilder& builder) :
name(builder.name), is_active(builder.is_active) {

    SDL_Rect r{};
    if (builder.parent == nullptr)
        r = convertRectToScreen(builder.Location);
    else {
        r.x = builder.parent->x + (builder.Location.x * builder.parent->w);
        r.y = builder.parent->y + (builder.Location.y * builder.parent->h);
        r.w = builder.Location.w * builder.parent->w;
        r.h = builder.Location.h * builder.parent->h;
    }
    raw_rect = r;
    const int radius = static_cast<int>(builder.curve * GlobalState::GameWindow_CurrentWidth);

    for (int i = 0; i < 4; i++) {
        const int x_ = r.x + ((i % 2) * r.w);
        const int y_ = r.y + ((i / 2) * r.h);
        GenerateCircle(&curved_edges[i], radius, {x_, y_}, i);
    }

    SDL_Rect left_side = r;
    left_side.x += 1;
    left_side.y += radius;
    left_side.w = radius;
    left_side.h -= radius * 2;

    SDL_Rect right_side = left_side;
    right_side.x += r.w - radius;
    right_side.w -= 2;
    // y / h is the same

    r.x += radius;
    r.y += 1;
    r.w -= (radius * 2) - 2;
    r.h -= 2;
    inner_rect = r;

    rects.push_back(r);
    rects.push_back(left_side);
    rects.push_back(right_side);

    outline_straight.push_back({r.x, r.y});
    outline_straight.push_back({r.x + r.w, r.y});
    outline_straight.push_back({r.x, r.y + r.h});
    outline_straight.push_back({r.x + r.w, r.y + r.h});
    outline_straight.push_back({left_side.x, left_side.y});
    outline_straight.push_back({left_side.x, left_side.y + left_side.h});
    outline_straight.push_back({right_side.x + right_side.w, right_side.y});
    outline_straight.push_back({right_side.x + right_side.w, right_side.y + right_side.h});

    // hacky, but it works
    for (const auto& p : curved_edges[0]) {
        if (p.x <= r.x && p.y <= left_side.y) {
            outline_curves.push_back(p);
        }
    }
    for (const auto& p : curved_edges[2]) {
        if (p.x <= r.x && p.y >= left_side.y + left_side.h) {
            outline_curves.push_back(p);
        }
    }

    for (const auto& p : curved_edges[1]) {
        if (p.x >= right_side.x && p.y <= right_side.y) {
            outline_curves.push_back(p);
        }
    }
    for (const auto& p : curved_edges[3]) {
        if (p.x >= right_side.x && p.y >= right_side.y + right_side.h) {
            outline_curves.push_back(p);
        }
    }

    for (const auto& t : builder.text_builders) {
        TextBuilder tb = t;
        const int w = t.calculateWidth();
        const int h = t.calculateHeight();
        const int x_margin = inner_rect.w * 0.015625; // 1/64 of width
        const int y_margin = inner_rect.h * 0.0625; // 1/16 of height

        switch (t.alignment_x) {
            case X_ALIGN_NONE: {
                tb.origin.x += inner_rect.x;
                break;
            }
            case X_ALIGN_LEFT: {
                tb.origin.x = inner_rect.x + x_margin;
                break;
            }
            case X_ALIGN_CENTER: {
                const int half_w = w / 2;
                const int half_rect_w = inner_rect.w / 2;
                tb.origin.x = inner_rect.x + half_rect_w - half_w;
                break;
            }
            case X_ALIGN_RIGHT: {
                tb.origin.x = inner_rect.x + inner_rect.w - w - x_margin;
                break;
            }
        }

        switch (t.alignment_y) {
            case Y_ALIGN_NONE: {
                tb.origin.y += inner_rect.y;
                break;
            }
            case Y_ALIGN_TOP: {
                tb.origin.y = inner_rect.y + y_margin;
                break;
            }
            case Y_ALIGN_MIDDLE: {
                const int half_h = h / 2;
                const int half_rect_h = inner_rect.h / 2;
                tb.origin.y = inner_rect.y + half_rect_h - half_h;
                break;
            }
            case Y_ALIGN_BOTTOM: {
                tb.origin.y = inner_rect.y + inner_rect.h - h - y_margin;
                break;
            }
        }
        tb.origin.x += tb.offset.x;
        tb.origin.y += tb.offset.y;
        text.emplace_back(t.font->GenerateText(tb));
    }

    for (auto& uib : builder.nodes) {
        uib.parent = &inner_rect;
        children.emplace_back(uib);
    }
}

SDL_Rect UIElement::convertRectToScreen(SDL_FRect r) {
    return { static_cast<int>(r.x * GlobalState::GameWindow_CurrentWidth),
             static_cast<int>(r.y * GlobalState::GameWindow_CurrentHeight),
             static_cast<int>(r.w * GlobalState::GameWindow_CurrentWidth),
             static_cast<int>(r.h * GlobalState::GameWindow_CurrentHeight)};
}

void UIElement::GenerateCircle(std::vector<SDL_Point> *points, int radius, SDL_Point origin, int corner) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius * 2);

    int x_offset = radius, y_offset = radius;
    // 0 - upper left (default)
    // 1 - upper right
    // 2 - lower left
    // 3 - lower right
    switch (corner) {
        case 1: {
            x_offset *= -1;
            break;
        }
        case 2: {
            y_offset *= -1;
            break;
        }
        case 3: {
            x_offset *= -1;
            y_offset *= -1;
            break;
        }
        default: break;
    }
    origin.x += x_offset;
    origin.y += y_offset;

    while (x >= y) {
        points->push_back({origin.x + x, origin.y + y});
        points->push_back({origin.x + x, origin.y - y});
        points->push_back({origin.x + y, origin.y + x});
        points->push_back({origin.x + y, origin.y - x});

        points->push_back({origin.x - x, origin.y + y});
        points->push_back({origin.x - x, origin.y - y});
        points->push_back({origin.x - y, origin.y + x});
        points->push_back({origin.x - y, origin.y - x});

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius * 2);
        }
    }
}

bool UIElement::isActive() const {
    return is_active;
}

void UIElement::Update() {
    // TODO
}

void UIElement::Draw(SpriteBatch *sb) const {
    sb->Add(this);
    for (const auto& c : children) {
        c.Draw(sb);
    }
}

void UIElement::setActiveState(bool state) {
    is_active = state;
}
