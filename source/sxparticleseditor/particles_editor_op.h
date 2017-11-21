
#ifndef __PARTICLES_EDITOR_OP_H
#define __PARTICLES_EDITOR_OP_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <particles/sxparticles.h>
#include <level/sxlevel.h>
#include <render/sxrender.h>

void SXParticlesEditor::EffInitList();

void SXParticlesEditor::EffVisible(bool visible, bool iscreate);

void SXParticlesEditor::EffNulling();

void SXParticlesEditor::EffDataInit();

void SXParticlesEditor::TabsVisible(bool visible);

void SXParticlesEditor::AllInTabsVisible(bool visible);

void SXParticlesEditor::AllInTabsNulling();

void SXParticlesEditor::EmitterCreateVisible(bool visible);

void SXParticlesEditor::BaseVisible(bool visible);

void SXParticlesEditor::BaseDataInit();

void SXParticlesEditor::BaseNulling();

void SXParticlesEditor::BoundVisible(bool visible);

void SXParticlesEditor::BoundDataInit();

void SXParticlesEditor::BoundNulling();

void SXParticlesEditor::CharacterVisible(bool visible);

void SXParticlesEditor::CharacterDataInit();

void SXParticlesEditor::CharacterNulling();

void SXParticlesEditor::SpawnVisible(bool visible);

void SXParticlesEditor::SpawnDataInit();

void SXParticlesEditor::SpawnNulling();

void SXParticlesEditor::TexVisible(bool visible);

void SXParticlesEditor::TexDataInit();

void SXParticlesEditor::TexNulling();

void SXParticlesEditor::VelocityAccVisible(bool visible);

void SXParticlesEditor::VelocityDataInit();

void SXParticlesEditor::VelocityAccNulling();

#endif