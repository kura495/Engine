import bpy
import math

# ブレンダーに登録するアドオン情報
bl_info = {
    "name": "レベルエディタ",
    "author": "Kuramoto Atsushi",
    "version": (1,0),
    "blender": (3,3,1),
    "location": "",
    "description": "レベルエディタ",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Object"
}

#オペレータ
# 頂点を伸ばす
class MYADDON_OT_stretch_vertex(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_steretch_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点座標を引っ張って伸ばします"
    bl_options = {'REGISTER', 'UNDO'}

    #メニューを実行した時に呼ばれるコールバック関数
    def execute(self, context):
        bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
        print("頂点を伸ばしました。")

        #オペレータの命令終了を通知
        return {'FINISHED'}

#ICO球を作成
class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_object"
    bl_label = "ICO球生成"
    bl_description = "ICO球を生成します"
    bl_options = {'REGISTER', 'UNDO'}

    #メニューを実行した時に呼ばれる関数
    def execute(self, context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました")

        return {'FINISHED'}

# シーン出力
class MYADDON_OT_export_scene(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をEXportします"

    def execute(self,context):
        print("シーン情報をExportします")
        # シーン内の全オブジェクトについて
        for object in bpy.context.scene.objects:
            # オブジェクトのタイプと名前を出力
            print(object.type + "-" + object.name)
            # トランスフォーム情報を出力
            trans, rot, scale = object.matrix_local.decompose()
            # QuaertanionからEulerに変換
            rot = rot.to_euler()
            # ラジアンから弧度法へ
            rot.x = math.degrees(rot.x)
            rot.y = math.degrees(rot.y)
            rot.z = math.degrees(rot.z)
            print("Trans(%f,%f,%f)"%(trans.x,trans.y,trans.z))
            print("Rot(%f,%f,%f)"%(rot.x,rot.y,rot.z))
            print("Scale(%f,%f,%f)"%(scale.x,scale.y,scale.z))
            # 親オブジェクトの名前の表示
            if object.parent:
                print("Parent:" + object.parent.name)
            # 区切りを入れる
            print()


        print("シーン情報をExportしました")
        self.report({'INFO'},"シーン情報をExportしました")
        return {'FINISHED'}

# トップバーの拡張メニュー
class TOPBAR_MT_my_menu(bpy.types.Menu):

    #Blenderがクラスを識別する為の固有の文字列
    bl_idname = "TOPBAR_MT_my_menu"
    #メニューのラベルとして表示される文字列
    bl_label = "MyMenu"
    #著者表示用の文字列
    bl_description = "拡張メニュー by " + bl_info["author"]

    #サブメニューの描画
    def draw(self, context):
        #トップバーの「エディターメニュー」に項目(オペレータ)を追加
        self.layout.operator("wm.url_open_preset",text = "Manual", icon = 'HELP')
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname, text = MYADDON_OT_stretch_vertex.bl_label)
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname, text = MYADDON_OT_create_ico_sphere.bl_label)
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,text = MYADDON_OT_export_scene.bl_label)


    # 既存のメニューにサブメニューを追加
    def submenu(self, context):
        #ID指定でサブメニューを追加
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)



# Blenderに登録するクラスリスト
classes = (
    TOPBAR_MT_my_menu,
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_export_scene,
)

# アドオン有効化時コールバック
def register():
    # Blenderに登録するクラス
    for cls in classes:
        bpy.utils.register_class(cls)

    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    print("レベルエディタが有効化されました。")

# アドオン無効化時コールバック
def unregister():
    #メニューに項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    # Blenderに登録するクラス
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効化されました。")
    
#test
if __name__ == "__main__":
    register()

