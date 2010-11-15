'Human Body Project - face memory browser
'Copyright (C) 2004  Bob Mottram
'
'This program is free software; you can redistribute it and/or modify
'it under the terms of the GNU General Public License as published by
'the Free Software Foundation; either version 2 of the License, or
'(at your option) any later version.
'
'This program is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'GNU General Public License for more details.
'
'You should have received a copy of the GNU General Public License
'along with this program; if not, write to the Free Software
'Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic


''' <summary>
''' The main form
''' </summary>
Class frmMain
    Inherits System.Windows.Forms.Form
#Region "Windows Form Designer generated code "
    Public Sub New()
        MyBase.New()
        If m_vb6FormDefInstance Is Nothing Then
            If m_InitializingDefInstance Then
                m_vb6FormDefInstance = Me
            Else
                Try
                    'For the start-up form, the first instance created is the default instance.
                    If System.Reflection.Assembly.GetExecutingAssembly.EntryPoint.DeclaringType Is Me.GetType Then
                        m_vb6FormDefInstance = Me
                    End If
                Catch
                End Try
            End If
        End If
        'This call is required by the Windows Form Designer.
        InitializeComponent()
    End Sub
    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
        If Disposing Then
            If Not components Is Nothing Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(Disposing)
    End Sub
    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer
    Public ToolTip1 As System.Windows.Forms.ToolTip
    Public WithEvents _cmdPicture_5 As System.Windows.Forms.Button
    Public WithEvents _cmdPicture_4 As System.Windows.Forms.Button
    Public WithEvents _cmdPicture_3 As System.Windows.Forms.Button
    Public WithEvents _cmdPicture_2 As System.Windows.Forms.Button
    Public WithEvents _cmdPicture_1 As System.Windows.Forms.Button
    Public WithEvents _cmdPicture_0 As System.Windows.Forms.Button
    Public WithEvents _picFace_339 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_338 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_337 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_336 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_335 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_334 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_333 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_332 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_331 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_330 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_329 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_328 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_327 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_326 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_325 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_324 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_323 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_322 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_321 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_320 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_319 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_318 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_317 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_316 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_315 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_314 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_313 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_312 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_311 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_310 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_309 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_308 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_307 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_306 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_305 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_304 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_303 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_302 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_301 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_300 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_299 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_298 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_297 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_296 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_295 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_294 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_293 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_292 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_291 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_290 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_289 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_288 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_287 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_286 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_285 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_284 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_283 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_282 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_281 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_280 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_279 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_278 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_277 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_276 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_275 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_274 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_273 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_272 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_271 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_270 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_269 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_268 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_267 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_266 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_265 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_264 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_263 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_262 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_261 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_260 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_259 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_258 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_257 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_256 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_255 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_254 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_253 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_252 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_251 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_250 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_249 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_248 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_247 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_246 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_245 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_244 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_243 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_242 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_241 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_240 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_239 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_238 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_237 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_236 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_235 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_234 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_233 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_232 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_231 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_230 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_229 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_228 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_227 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_226 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_225 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_224 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_223 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_222 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_221 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_220 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_219 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_218 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_217 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_216 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_215 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_214 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_213 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_212 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_211 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_210 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_209 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_208 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_207 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_206 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_205 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_204 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_203 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_202 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_201 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_200 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_199 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_198 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_197 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_196 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_195 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_194 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_193 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_192 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_191 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_190 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_189 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_188 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_187 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_186 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_185 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_184 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_183 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_182 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_181 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_180 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_179 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_178 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_177 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_176 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_175 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_174 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_173 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_172 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_171 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_170 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_169 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_168 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_167 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_166 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_165 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_164 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_163 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_162 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_161 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_160 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_159 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_158 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_157 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_156 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_155 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_154 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_153 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_152 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_151 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_150 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_149 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_148 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_147 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_146 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_145 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_144 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_143 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_142 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_141 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_140 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_139 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_138 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_137 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_136 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_135 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_134 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_133 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_132 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_131 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_130 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_129 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_128 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_127 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_126 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_125 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_124 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_123 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_122 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_121 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_120 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_119 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_118 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_117 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_116 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_115 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_114 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_113 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_112 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_111 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_110 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_109 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_108 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_107 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_106 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_105 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_104 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_103 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_102 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_101 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_100 As System.Windows.Forms.PictureBox
    Public WithEvents CommonDialog1 As AxMSComDlg.AxCommonDialog
    Public WithEvents cmdClear As System.Windows.Forms.Button
    Public WithEvents _cmdName_5 As System.Windows.Forms.Button
    Public WithEvents _cmdName_4 As System.Windows.Forms.Button
    Public WithEvents _cmdName_3 As System.Windows.Forms.Button
    Public WithEvents _cmdName_2 As System.Windows.Forms.Button
    Public WithEvents _cmdName_1 As System.Windows.Forms.Button
    Public WithEvents _cmdName_0 As System.Windows.Forms.Button
    Public WithEvents _txtName_5 As System.Windows.Forms.TextBox
    Public WithEvents _txtName_4 As System.Windows.Forms.TextBox
    Public WithEvents _txtName_3 As System.Windows.Forms.TextBox
    Public WithEvents _txtName_2 As System.Windows.Forms.TextBox
    Public WithEvents _txtName_1 As System.Windows.Forms.TextBox
    Public WithEvents _txtName_0 As System.Windows.Forms.TextBox
    Public WithEvents _picFace_99 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_98 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_97 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_96 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_95 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_94 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_93 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_92 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_91 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_90 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_89 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_88 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_87 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_86 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_85 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_84 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_83 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_82 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_81 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_80 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_79 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_78 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_77 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_76 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_75 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_74 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_73 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_72 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_71 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_70 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_69 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_68 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_67 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_66 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_65 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_64 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_63 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_62 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_61 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_60 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_59 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_58 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_57 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_56 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_55 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_54 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_53 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_52 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_51 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_50 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_49 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_48 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_47 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_46 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_45 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_44 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_43 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_42 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_41 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_40 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_39 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_38 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_37 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_36 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_35 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_34 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_33 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_32 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_31 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_30 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_29 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_28 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_27 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_26 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_25 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_24 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_23 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_22 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_21 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_20 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_19 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_18 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_17 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_16 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_15 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_14 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_13 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_12 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_11 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_10 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_9 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_8 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_7 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_6 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_5 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_4 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_3 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_2 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_1 As System.Windows.Forms.PictureBox
    Public WithEvents _picFace_0 As System.Windows.Forms.PictureBox
    Public WithEvents imgPortrait As System.Windows.Forms.PictureBox
    Public WithEvents Label1 As System.Windows.Forms.Label
    Public WithEvents cmdName As Microsoft.VisualBasic.Compatibility.VB6.ButtonArray
    Public WithEvents cmdPicture As Microsoft.VisualBasic.Compatibility.VB6.ButtonArray
    Public WithEvents picFace As Microsoft.VisualBasic.Compatibility.VB6.PictureBoxArray
    Public WithEvents txtName As Microsoft.VisualBasic.Compatibility.VB6.TextBoxArray
    Public WithEvents mnuOpen As System.Windows.Forms.MenuItem
    Public WithEvents mnuSaveAs As System.Windows.Forms.MenuItem
    Public WithEvents mnuFile As System.Windows.Forms.MenuItem
    Public MainMenu1 As System.Windows.Forms.MainMenu
    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmMain))
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me._cmdPicture_5 = New System.Windows.Forms.Button
        Me._cmdPicture_4 = New System.Windows.Forms.Button
        Me._cmdPicture_3 = New System.Windows.Forms.Button
        Me._cmdPicture_2 = New System.Windows.Forms.Button
        Me._cmdPicture_1 = New System.Windows.Forms.Button
        Me._cmdPicture_0 = New System.Windows.Forms.Button
        Me._cmdName_0 = New System.Windows.Forms.Button
        Me._picFace_339 = New System.Windows.Forms.PictureBox
        Me._picFace_338 = New System.Windows.Forms.PictureBox
        Me._picFace_337 = New System.Windows.Forms.PictureBox
        Me._picFace_336 = New System.Windows.Forms.PictureBox
        Me._picFace_335 = New System.Windows.Forms.PictureBox
        Me._picFace_334 = New System.Windows.Forms.PictureBox
        Me._picFace_333 = New System.Windows.Forms.PictureBox
        Me._picFace_332 = New System.Windows.Forms.PictureBox
        Me._picFace_331 = New System.Windows.Forms.PictureBox
        Me._picFace_330 = New System.Windows.Forms.PictureBox
        Me._picFace_329 = New System.Windows.Forms.PictureBox
        Me._picFace_328 = New System.Windows.Forms.PictureBox
        Me._picFace_327 = New System.Windows.Forms.PictureBox
        Me._picFace_326 = New System.Windows.Forms.PictureBox
        Me._picFace_325 = New System.Windows.Forms.PictureBox
        Me._picFace_324 = New System.Windows.Forms.PictureBox
        Me._picFace_323 = New System.Windows.Forms.PictureBox
        Me._picFace_322 = New System.Windows.Forms.PictureBox
        Me._picFace_321 = New System.Windows.Forms.PictureBox
        Me._picFace_320 = New System.Windows.Forms.PictureBox
        Me._picFace_319 = New System.Windows.Forms.PictureBox
        Me._picFace_318 = New System.Windows.Forms.PictureBox
        Me._picFace_317 = New System.Windows.Forms.PictureBox
        Me._picFace_316 = New System.Windows.Forms.PictureBox
        Me._picFace_315 = New System.Windows.Forms.PictureBox
        Me._picFace_314 = New System.Windows.Forms.PictureBox
        Me._picFace_313 = New System.Windows.Forms.PictureBox
        Me._picFace_312 = New System.Windows.Forms.PictureBox
        Me._picFace_311 = New System.Windows.Forms.PictureBox
        Me._picFace_310 = New System.Windows.Forms.PictureBox
        Me._picFace_309 = New System.Windows.Forms.PictureBox
        Me._picFace_308 = New System.Windows.Forms.PictureBox
        Me._picFace_307 = New System.Windows.Forms.PictureBox
        Me._picFace_306 = New System.Windows.Forms.PictureBox
        Me._picFace_305 = New System.Windows.Forms.PictureBox
        Me._picFace_304 = New System.Windows.Forms.PictureBox
        Me._picFace_303 = New System.Windows.Forms.PictureBox
        Me._picFace_302 = New System.Windows.Forms.PictureBox
        Me._picFace_301 = New System.Windows.Forms.PictureBox
        Me._picFace_300 = New System.Windows.Forms.PictureBox
        Me._picFace_299 = New System.Windows.Forms.PictureBox
        Me._picFace_298 = New System.Windows.Forms.PictureBox
        Me._picFace_297 = New System.Windows.Forms.PictureBox
        Me._picFace_296 = New System.Windows.Forms.PictureBox
        Me._picFace_295 = New System.Windows.Forms.PictureBox
        Me._picFace_294 = New System.Windows.Forms.PictureBox
        Me._picFace_293 = New System.Windows.Forms.PictureBox
        Me._picFace_292 = New System.Windows.Forms.PictureBox
        Me._picFace_291 = New System.Windows.Forms.PictureBox
        Me._picFace_290 = New System.Windows.Forms.PictureBox
        Me._picFace_289 = New System.Windows.Forms.PictureBox
        Me._picFace_288 = New System.Windows.Forms.PictureBox
        Me._picFace_287 = New System.Windows.Forms.PictureBox
        Me._picFace_286 = New System.Windows.Forms.PictureBox
        Me._picFace_285 = New System.Windows.Forms.PictureBox
        Me._picFace_284 = New System.Windows.Forms.PictureBox
        Me._picFace_283 = New System.Windows.Forms.PictureBox
        Me._picFace_282 = New System.Windows.Forms.PictureBox
        Me._picFace_281 = New System.Windows.Forms.PictureBox
        Me._picFace_280 = New System.Windows.Forms.PictureBox
        Me._picFace_279 = New System.Windows.Forms.PictureBox
        Me._picFace_278 = New System.Windows.Forms.PictureBox
        Me._picFace_277 = New System.Windows.Forms.PictureBox
        Me._picFace_276 = New System.Windows.Forms.PictureBox
        Me._picFace_275 = New System.Windows.Forms.PictureBox
        Me._picFace_274 = New System.Windows.Forms.PictureBox
        Me._picFace_273 = New System.Windows.Forms.PictureBox
        Me._picFace_272 = New System.Windows.Forms.PictureBox
        Me._picFace_271 = New System.Windows.Forms.PictureBox
        Me._picFace_270 = New System.Windows.Forms.PictureBox
        Me._picFace_269 = New System.Windows.Forms.PictureBox
        Me._picFace_268 = New System.Windows.Forms.PictureBox
        Me._picFace_267 = New System.Windows.Forms.PictureBox
        Me._picFace_266 = New System.Windows.Forms.PictureBox
        Me._picFace_265 = New System.Windows.Forms.PictureBox
        Me._picFace_264 = New System.Windows.Forms.PictureBox
        Me._picFace_263 = New System.Windows.Forms.PictureBox
        Me._picFace_262 = New System.Windows.Forms.PictureBox
        Me._picFace_261 = New System.Windows.Forms.PictureBox
        Me._picFace_260 = New System.Windows.Forms.PictureBox
        Me._picFace_259 = New System.Windows.Forms.PictureBox
        Me._picFace_258 = New System.Windows.Forms.PictureBox
        Me._picFace_257 = New System.Windows.Forms.PictureBox
        Me._picFace_256 = New System.Windows.Forms.PictureBox
        Me._picFace_255 = New System.Windows.Forms.PictureBox
        Me._picFace_254 = New System.Windows.Forms.PictureBox
        Me._picFace_253 = New System.Windows.Forms.PictureBox
        Me._picFace_252 = New System.Windows.Forms.PictureBox
        Me._picFace_251 = New System.Windows.Forms.PictureBox
        Me._picFace_250 = New System.Windows.Forms.PictureBox
        Me._picFace_249 = New System.Windows.Forms.PictureBox
        Me._picFace_248 = New System.Windows.Forms.PictureBox
        Me._picFace_247 = New System.Windows.Forms.PictureBox
        Me._picFace_246 = New System.Windows.Forms.PictureBox
        Me._picFace_245 = New System.Windows.Forms.PictureBox
        Me._picFace_244 = New System.Windows.Forms.PictureBox
        Me._picFace_243 = New System.Windows.Forms.PictureBox
        Me._picFace_242 = New System.Windows.Forms.PictureBox
        Me._picFace_241 = New System.Windows.Forms.PictureBox
        Me._picFace_240 = New System.Windows.Forms.PictureBox
        Me._picFace_239 = New System.Windows.Forms.PictureBox
        Me._picFace_238 = New System.Windows.Forms.PictureBox
        Me._picFace_237 = New System.Windows.Forms.PictureBox
        Me._picFace_236 = New System.Windows.Forms.PictureBox
        Me._picFace_235 = New System.Windows.Forms.PictureBox
        Me._picFace_234 = New System.Windows.Forms.PictureBox
        Me._picFace_233 = New System.Windows.Forms.PictureBox
        Me._picFace_232 = New System.Windows.Forms.PictureBox
        Me._picFace_231 = New System.Windows.Forms.PictureBox
        Me._picFace_230 = New System.Windows.Forms.PictureBox
        Me._picFace_229 = New System.Windows.Forms.PictureBox
        Me._picFace_228 = New System.Windows.Forms.PictureBox
        Me._picFace_227 = New System.Windows.Forms.PictureBox
        Me._picFace_226 = New System.Windows.Forms.PictureBox
        Me._picFace_225 = New System.Windows.Forms.PictureBox
        Me._picFace_224 = New System.Windows.Forms.PictureBox
        Me._picFace_223 = New System.Windows.Forms.PictureBox
        Me._picFace_222 = New System.Windows.Forms.PictureBox
        Me._picFace_221 = New System.Windows.Forms.PictureBox
        Me._picFace_220 = New System.Windows.Forms.PictureBox
        Me._picFace_219 = New System.Windows.Forms.PictureBox
        Me._picFace_218 = New System.Windows.Forms.PictureBox
        Me._picFace_217 = New System.Windows.Forms.PictureBox
        Me._picFace_216 = New System.Windows.Forms.PictureBox
        Me._picFace_215 = New System.Windows.Forms.PictureBox
        Me._picFace_214 = New System.Windows.Forms.PictureBox
        Me._picFace_213 = New System.Windows.Forms.PictureBox
        Me._picFace_212 = New System.Windows.Forms.PictureBox
        Me._picFace_211 = New System.Windows.Forms.PictureBox
        Me._picFace_210 = New System.Windows.Forms.PictureBox
        Me._picFace_209 = New System.Windows.Forms.PictureBox
        Me._picFace_208 = New System.Windows.Forms.PictureBox
        Me._picFace_207 = New System.Windows.Forms.PictureBox
        Me._picFace_206 = New System.Windows.Forms.PictureBox
        Me._picFace_205 = New System.Windows.Forms.PictureBox
        Me._picFace_204 = New System.Windows.Forms.PictureBox
        Me._picFace_203 = New System.Windows.Forms.PictureBox
        Me._picFace_202 = New System.Windows.Forms.PictureBox
        Me._picFace_201 = New System.Windows.Forms.PictureBox
        Me._picFace_200 = New System.Windows.Forms.PictureBox
        Me._picFace_199 = New System.Windows.Forms.PictureBox
        Me._picFace_198 = New System.Windows.Forms.PictureBox
        Me._picFace_197 = New System.Windows.Forms.PictureBox
        Me._picFace_196 = New System.Windows.Forms.PictureBox
        Me._picFace_195 = New System.Windows.Forms.PictureBox
        Me._picFace_194 = New System.Windows.Forms.PictureBox
        Me._picFace_193 = New System.Windows.Forms.PictureBox
        Me._picFace_192 = New System.Windows.Forms.PictureBox
        Me._picFace_191 = New System.Windows.Forms.PictureBox
        Me._picFace_190 = New System.Windows.Forms.PictureBox
        Me._picFace_189 = New System.Windows.Forms.PictureBox
        Me._picFace_188 = New System.Windows.Forms.PictureBox
        Me._picFace_187 = New System.Windows.Forms.PictureBox
        Me._picFace_186 = New System.Windows.Forms.PictureBox
        Me._picFace_185 = New System.Windows.Forms.PictureBox
        Me._picFace_184 = New System.Windows.Forms.PictureBox
        Me._picFace_183 = New System.Windows.Forms.PictureBox
        Me._picFace_182 = New System.Windows.Forms.PictureBox
        Me._picFace_181 = New System.Windows.Forms.PictureBox
        Me._picFace_180 = New System.Windows.Forms.PictureBox
        Me._picFace_179 = New System.Windows.Forms.PictureBox
        Me._picFace_178 = New System.Windows.Forms.PictureBox
        Me._picFace_177 = New System.Windows.Forms.PictureBox
        Me._picFace_176 = New System.Windows.Forms.PictureBox
        Me._picFace_175 = New System.Windows.Forms.PictureBox
        Me._picFace_174 = New System.Windows.Forms.PictureBox
        Me._picFace_173 = New System.Windows.Forms.PictureBox
        Me._picFace_172 = New System.Windows.Forms.PictureBox
        Me._picFace_171 = New System.Windows.Forms.PictureBox
        Me._picFace_170 = New System.Windows.Forms.PictureBox
        Me._picFace_169 = New System.Windows.Forms.PictureBox
        Me._picFace_168 = New System.Windows.Forms.PictureBox
        Me._picFace_167 = New System.Windows.Forms.PictureBox
        Me._picFace_166 = New System.Windows.Forms.PictureBox
        Me._picFace_165 = New System.Windows.Forms.PictureBox
        Me._picFace_164 = New System.Windows.Forms.PictureBox
        Me._picFace_163 = New System.Windows.Forms.PictureBox
        Me._picFace_162 = New System.Windows.Forms.PictureBox
        Me._picFace_161 = New System.Windows.Forms.PictureBox
        Me._picFace_160 = New System.Windows.Forms.PictureBox
        Me._picFace_159 = New System.Windows.Forms.PictureBox
        Me._picFace_158 = New System.Windows.Forms.PictureBox
        Me._picFace_157 = New System.Windows.Forms.PictureBox
        Me._picFace_156 = New System.Windows.Forms.PictureBox
        Me._picFace_155 = New System.Windows.Forms.PictureBox
        Me._picFace_154 = New System.Windows.Forms.PictureBox
        Me._picFace_153 = New System.Windows.Forms.PictureBox
        Me._picFace_152 = New System.Windows.Forms.PictureBox
        Me._picFace_151 = New System.Windows.Forms.PictureBox
        Me._picFace_150 = New System.Windows.Forms.PictureBox
        Me._picFace_149 = New System.Windows.Forms.PictureBox
        Me._picFace_148 = New System.Windows.Forms.PictureBox
        Me._picFace_147 = New System.Windows.Forms.PictureBox
        Me._picFace_146 = New System.Windows.Forms.PictureBox
        Me._picFace_145 = New System.Windows.Forms.PictureBox
        Me._picFace_144 = New System.Windows.Forms.PictureBox
        Me._picFace_143 = New System.Windows.Forms.PictureBox
        Me._picFace_142 = New System.Windows.Forms.PictureBox
        Me._picFace_141 = New System.Windows.Forms.PictureBox
        Me._picFace_140 = New System.Windows.Forms.PictureBox
        Me._picFace_139 = New System.Windows.Forms.PictureBox
        Me._picFace_138 = New System.Windows.Forms.PictureBox
        Me._picFace_137 = New System.Windows.Forms.PictureBox
        Me._picFace_136 = New System.Windows.Forms.PictureBox
        Me._picFace_135 = New System.Windows.Forms.PictureBox
        Me._picFace_134 = New System.Windows.Forms.PictureBox
        Me._picFace_133 = New System.Windows.Forms.PictureBox
        Me._picFace_132 = New System.Windows.Forms.PictureBox
        Me._picFace_131 = New System.Windows.Forms.PictureBox
        Me._picFace_130 = New System.Windows.Forms.PictureBox
        Me._picFace_129 = New System.Windows.Forms.PictureBox
        Me._picFace_128 = New System.Windows.Forms.PictureBox
        Me._picFace_127 = New System.Windows.Forms.PictureBox
        Me._picFace_126 = New System.Windows.Forms.PictureBox
        Me._picFace_125 = New System.Windows.Forms.PictureBox
        Me._picFace_124 = New System.Windows.Forms.PictureBox
        Me._picFace_123 = New System.Windows.Forms.PictureBox
        Me._picFace_122 = New System.Windows.Forms.PictureBox
        Me._picFace_121 = New System.Windows.Forms.PictureBox
        Me._picFace_120 = New System.Windows.Forms.PictureBox
        Me._picFace_119 = New System.Windows.Forms.PictureBox
        Me._picFace_118 = New System.Windows.Forms.PictureBox
        Me._picFace_117 = New System.Windows.Forms.PictureBox
        Me._picFace_116 = New System.Windows.Forms.PictureBox
        Me._picFace_115 = New System.Windows.Forms.PictureBox
        Me._picFace_114 = New System.Windows.Forms.PictureBox
        Me._picFace_113 = New System.Windows.Forms.PictureBox
        Me._picFace_112 = New System.Windows.Forms.PictureBox
        Me._picFace_111 = New System.Windows.Forms.PictureBox
        Me._picFace_110 = New System.Windows.Forms.PictureBox
        Me._picFace_109 = New System.Windows.Forms.PictureBox
        Me._picFace_108 = New System.Windows.Forms.PictureBox
        Me._picFace_107 = New System.Windows.Forms.PictureBox
        Me._picFace_106 = New System.Windows.Forms.PictureBox
        Me._picFace_105 = New System.Windows.Forms.PictureBox
        Me._picFace_104 = New System.Windows.Forms.PictureBox
        Me._picFace_103 = New System.Windows.Forms.PictureBox
        Me._picFace_102 = New System.Windows.Forms.PictureBox
        Me._picFace_101 = New System.Windows.Forms.PictureBox
        Me._picFace_100 = New System.Windows.Forms.PictureBox
        Me.CommonDialog1 = New AxMSComDlg.AxCommonDialog
        Me.cmdClear = New System.Windows.Forms.Button
        Me._cmdName_5 = New System.Windows.Forms.Button
        Me._cmdName_4 = New System.Windows.Forms.Button
        Me._cmdName_3 = New System.Windows.Forms.Button
        Me._cmdName_2 = New System.Windows.Forms.Button
        Me._cmdName_1 = New System.Windows.Forms.Button
        Me._txtName_5 = New System.Windows.Forms.TextBox
        Me._txtName_4 = New System.Windows.Forms.TextBox
        Me._txtName_3 = New System.Windows.Forms.TextBox
        Me._txtName_2 = New System.Windows.Forms.TextBox
        Me._txtName_1 = New System.Windows.Forms.TextBox
        Me._txtName_0 = New System.Windows.Forms.TextBox
        Me._picFace_99 = New System.Windows.Forms.PictureBox
        Me._picFace_98 = New System.Windows.Forms.PictureBox
        Me._picFace_97 = New System.Windows.Forms.PictureBox
        Me._picFace_96 = New System.Windows.Forms.PictureBox
        Me._picFace_95 = New System.Windows.Forms.PictureBox
        Me._picFace_94 = New System.Windows.Forms.PictureBox
        Me._picFace_93 = New System.Windows.Forms.PictureBox
        Me._picFace_92 = New System.Windows.Forms.PictureBox
        Me._picFace_91 = New System.Windows.Forms.PictureBox
        Me._picFace_90 = New System.Windows.Forms.PictureBox
        Me._picFace_89 = New System.Windows.Forms.PictureBox
        Me._picFace_88 = New System.Windows.Forms.PictureBox
        Me._picFace_87 = New System.Windows.Forms.PictureBox
        Me._picFace_86 = New System.Windows.Forms.PictureBox
        Me._picFace_85 = New System.Windows.Forms.PictureBox
        Me._picFace_84 = New System.Windows.Forms.PictureBox
        Me._picFace_83 = New System.Windows.Forms.PictureBox
        Me._picFace_82 = New System.Windows.Forms.PictureBox
        Me._picFace_81 = New System.Windows.Forms.PictureBox
        Me._picFace_80 = New System.Windows.Forms.PictureBox
        Me._picFace_79 = New System.Windows.Forms.PictureBox
        Me._picFace_78 = New System.Windows.Forms.PictureBox
        Me._picFace_77 = New System.Windows.Forms.PictureBox
        Me._picFace_76 = New System.Windows.Forms.PictureBox
        Me._picFace_75 = New System.Windows.Forms.PictureBox
        Me._picFace_74 = New System.Windows.Forms.PictureBox
        Me._picFace_73 = New System.Windows.Forms.PictureBox
        Me._picFace_72 = New System.Windows.Forms.PictureBox
        Me._picFace_71 = New System.Windows.Forms.PictureBox
        Me._picFace_70 = New System.Windows.Forms.PictureBox
        Me._picFace_69 = New System.Windows.Forms.PictureBox
        Me._picFace_68 = New System.Windows.Forms.PictureBox
        Me._picFace_67 = New System.Windows.Forms.PictureBox
        Me._picFace_66 = New System.Windows.Forms.PictureBox
        Me._picFace_65 = New System.Windows.Forms.PictureBox
        Me._picFace_64 = New System.Windows.Forms.PictureBox
        Me._picFace_63 = New System.Windows.Forms.PictureBox
        Me._picFace_62 = New System.Windows.Forms.PictureBox
        Me._picFace_61 = New System.Windows.Forms.PictureBox
        Me._picFace_60 = New System.Windows.Forms.PictureBox
        Me._picFace_59 = New System.Windows.Forms.PictureBox
        Me._picFace_58 = New System.Windows.Forms.PictureBox
        Me._picFace_57 = New System.Windows.Forms.PictureBox
        Me._picFace_56 = New System.Windows.Forms.PictureBox
        Me._picFace_55 = New System.Windows.Forms.PictureBox
        Me._picFace_54 = New System.Windows.Forms.PictureBox
        Me._picFace_53 = New System.Windows.Forms.PictureBox
        Me._picFace_52 = New System.Windows.Forms.PictureBox
        Me._picFace_51 = New System.Windows.Forms.PictureBox
        Me._picFace_50 = New System.Windows.Forms.PictureBox
        Me._picFace_49 = New System.Windows.Forms.PictureBox
        Me._picFace_48 = New System.Windows.Forms.PictureBox
        Me._picFace_47 = New System.Windows.Forms.PictureBox
        Me._picFace_46 = New System.Windows.Forms.PictureBox
        Me._picFace_45 = New System.Windows.Forms.PictureBox
        Me._picFace_44 = New System.Windows.Forms.PictureBox
        Me._picFace_43 = New System.Windows.Forms.PictureBox
        Me._picFace_42 = New System.Windows.Forms.PictureBox
        Me._picFace_41 = New System.Windows.Forms.PictureBox
        Me._picFace_40 = New System.Windows.Forms.PictureBox
        Me._picFace_39 = New System.Windows.Forms.PictureBox
        Me._picFace_38 = New System.Windows.Forms.PictureBox
        Me._picFace_37 = New System.Windows.Forms.PictureBox
        Me._picFace_36 = New System.Windows.Forms.PictureBox
        Me._picFace_35 = New System.Windows.Forms.PictureBox
        Me._picFace_34 = New System.Windows.Forms.PictureBox
        Me._picFace_33 = New System.Windows.Forms.PictureBox
        Me._picFace_32 = New System.Windows.Forms.PictureBox
        Me._picFace_31 = New System.Windows.Forms.PictureBox
        Me._picFace_30 = New System.Windows.Forms.PictureBox
        Me._picFace_29 = New System.Windows.Forms.PictureBox
        Me._picFace_28 = New System.Windows.Forms.PictureBox
        Me._picFace_27 = New System.Windows.Forms.PictureBox
        Me._picFace_26 = New System.Windows.Forms.PictureBox
        Me._picFace_25 = New System.Windows.Forms.PictureBox
        Me._picFace_24 = New System.Windows.Forms.PictureBox
        Me._picFace_23 = New System.Windows.Forms.PictureBox
        Me._picFace_22 = New System.Windows.Forms.PictureBox
        Me._picFace_21 = New System.Windows.Forms.PictureBox
        Me._picFace_20 = New System.Windows.Forms.PictureBox
        Me._picFace_19 = New System.Windows.Forms.PictureBox
        Me._picFace_18 = New System.Windows.Forms.PictureBox
        Me._picFace_17 = New System.Windows.Forms.PictureBox
        Me._picFace_16 = New System.Windows.Forms.PictureBox
        Me._picFace_15 = New System.Windows.Forms.PictureBox
        Me._picFace_14 = New System.Windows.Forms.PictureBox
        Me._picFace_13 = New System.Windows.Forms.PictureBox
        Me._picFace_12 = New System.Windows.Forms.PictureBox
        Me._picFace_11 = New System.Windows.Forms.PictureBox
        Me._picFace_10 = New System.Windows.Forms.PictureBox
        Me._picFace_9 = New System.Windows.Forms.PictureBox
        Me._picFace_8 = New System.Windows.Forms.PictureBox
        Me._picFace_7 = New System.Windows.Forms.PictureBox
        Me._picFace_6 = New System.Windows.Forms.PictureBox
        Me._picFace_5 = New System.Windows.Forms.PictureBox
        Me._picFace_4 = New System.Windows.Forms.PictureBox
        Me._picFace_3 = New System.Windows.Forms.PictureBox
        Me._picFace_2 = New System.Windows.Forms.PictureBox
        Me._picFace_1 = New System.Windows.Forms.PictureBox
        Me._picFace_0 = New System.Windows.Forms.PictureBox
        Me.imgPortrait = New System.Windows.Forms.PictureBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.cmdName = New Microsoft.VisualBasic.Compatibility.VB6.ButtonArray(Me.components)
        Me.cmdPicture = New Microsoft.VisualBasic.Compatibility.VB6.ButtonArray(Me.components)
        Me.picFace = New Microsoft.VisualBasic.Compatibility.VB6.PictureBoxArray(Me.components)
        Me.txtName = New Microsoft.VisualBasic.Compatibility.VB6.TextBoxArray(Me.components)
        Me.MainMenu1 = New System.Windows.Forms.MainMenu
        Me.mnuFile = New System.Windows.Forms.MenuItem
        Me.mnuOpen = New System.Windows.Forms.MenuItem
        Me.mnuSaveAs = New System.Windows.Forms.MenuItem
        CType(Me.CommonDialog1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.cmdName, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.cmdPicture, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.picFace, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.txtName, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        '_cmdPicture_5
        '
        Me._cmdPicture_5.BackColor = System.Drawing.SystemColors.Control
        Me._cmdPicture_5.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdPicture_5.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdPicture_5.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdPicture.SetIndex(Me._cmdPicture_5, CType(5, Short))
        Me._cmdPicture_5.Location = New System.Drawing.Point(152, 184)
        Me._cmdPicture_5.Name = "_cmdPicture_5"
        Me._cmdPicture_5.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdPicture_5.Size = New System.Drawing.Size(33, 25)
        Me._cmdPicture_5.TabIndex = 359
        Me._cmdPicture_5.Text = "pic"
        Me.ToolTip1.SetToolTip(Me._cmdPicture_5, "Add a picture for this person")
        '
        '_cmdPicture_4
        '
        Me._cmdPicture_4.BackColor = System.Drawing.SystemColors.Control
        Me._cmdPicture_4.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdPicture_4.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdPicture_4.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdPicture.SetIndex(Me._cmdPicture_4, CType(4, Short))
        Me._cmdPicture_4.Location = New System.Drawing.Point(152, 160)
        Me._cmdPicture_4.Name = "_cmdPicture_4"
        Me._cmdPicture_4.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdPicture_4.Size = New System.Drawing.Size(33, 25)
        Me._cmdPicture_4.TabIndex = 358
        Me._cmdPicture_4.Text = "pic"
        Me.ToolTip1.SetToolTip(Me._cmdPicture_4, "Add a picture for this person")
        '
        '_cmdPicture_3
        '
        Me._cmdPicture_3.BackColor = System.Drawing.SystemColors.Control
        Me._cmdPicture_3.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdPicture_3.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdPicture_3.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdPicture.SetIndex(Me._cmdPicture_3, CType(3, Short))
        Me._cmdPicture_3.Location = New System.Drawing.Point(152, 136)
        Me._cmdPicture_3.Name = "_cmdPicture_3"
        Me._cmdPicture_3.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdPicture_3.Size = New System.Drawing.Size(33, 25)
        Me._cmdPicture_3.TabIndex = 357
        Me._cmdPicture_3.Text = "pic"
        Me.ToolTip1.SetToolTip(Me._cmdPicture_3, "Add a picture for this person")
        '
        '_cmdPicture_2
        '
        Me._cmdPicture_2.BackColor = System.Drawing.SystemColors.Control
        Me._cmdPicture_2.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdPicture_2.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdPicture_2.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdPicture.SetIndex(Me._cmdPicture_2, CType(2, Short))
        Me._cmdPicture_2.Location = New System.Drawing.Point(152, 112)
        Me._cmdPicture_2.Name = "_cmdPicture_2"
        Me._cmdPicture_2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdPicture_2.Size = New System.Drawing.Size(33, 25)
        Me._cmdPicture_2.TabIndex = 356
        Me._cmdPicture_2.Text = "pic"
        Me.ToolTip1.SetToolTip(Me._cmdPicture_2, "Add a picture for this person")
        '
        '_cmdPicture_1
        '
        Me._cmdPicture_1.BackColor = System.Drawing.SystemColors.Control
        Me._cmdPicture_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdPicture_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdPicture_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdPicture.SetIndex(Me._cmdPicture_1, CType(1, Short))
        Me._cmdPicture_1.Location = New System.Drawing.Point(152, 88)
        Me._cmdPicture_1.Name = "_cmdPicture_1"
        Me._cmdPicture_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdPicture_1.Size = New System.Drawing.Size(33, 25)
        Me._cmdPicture_1.TabIndex = 355
        Me._cmdPicture_1.Text = "pic"
        Me.ToolTip1.SetToolTip(Me._cmdPicture_1, "Add a picture for this person")
        '
        '_cmdPicture_0
        '
        Me._cmdPicture_0.BackColor = System.Drawing.SystemColors.Control
        Me._cmdPicture_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdPicture_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdPicture_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdPicture.SetIndex(Me._cmdPicture_0, CType(0, Short))
        Me._cmdPicture_0.Location = New System.Drawing.Point(152, 64)
        Me._cmdPicture_0.Name = "_cmdPicture_0"
        Me._cmdPicture_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdPicture_0.Size = New System.Drawing.Size(33, 25)
        Me._cmdPicture_0.TabIndex = 354
        Me._cmdPicture_0.Text = "pic"
        Me.ToolTip1.SetToolTip(Me._cmdPicture_0, "Add a picture for this person")
        '
        '_cmdName_0
        '
        Me._cmdName_0.BackColor = System.Drawing.SystemColors.Control
        Me._cmdName_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdName_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdName_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdName.SetIndex(Me._cmdName_0, CType(0, Short))
        Me._cmdName_0.Location = New System.Drawing.Point(8, 64)
        Me._cmdName_0.Name = "_cmdName_0"
        Me._cmdName_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdName_0.Size = New System.Drawing.Size(17, 17)
        Me._cmdName_0.TabIndex = 106
        Me.ToolTip1.SetToolTip(Me._cmdName_0, "Select areas of the map for this person")
        '
        '_picFace_339
        '
        Me._picFace_339.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_339.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_339.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_339.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_339.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_339.Image = CType(resources.GetObject("_picFace_339.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_339, CType(339, Short))
        Me._picFace_339.Location = New System.Drawing.Point(192, 648)
        Me._picFace_339.Name = "_picFace_339"
        Me._picFace_339.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_339.Size = New System.Drawing.Size(41, 41)
        Me._picFace_339.TabIndex = 353
        Me._picFace_339.TabStop = False
        '
        '_picFace_338
        '
        Me._picFace_338.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_338.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_338.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_338.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_338.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_338.Image = CType(resources.GetObject("_picFace_338.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_338, CType(338, Short))
        Me._picFace_338.Location = New System.Drawing.Point(232, 648)
        Me._picFace_338.Name = "_picFace_338"
        Me._picFace_338.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_338.Size = New System.Drawing.Size(41, 41)
        Me._picFace_338.TabIndex = 352
        Me._picFace_338.TabStop = False
        '
        '_picFace_337
        '
        Me._picFace_337.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_337.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_337.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_337.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_337.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_337.Image = CType(resources.GetObject("_picFace_337.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_337, CType(337, Short))
        Me._picFace_337.Location = New System.Drawing.Point(272, 648)
        Me._picFace_337.Name = "_picFace_337"
        Me._picFace_337.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_337.Size = New System.Drawing.Size(41, 41)
        Me._picFace_337.TabIndex = 351
        Me._picFace_337.TabStop = False
        '
        '_picFace_336
        '
        Me._picFace_336.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_336.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_336.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_336.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_336.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_336.Image = CType(resources.GetObject("_picFace_336.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_336, CType(336, Short))
        Me._picFace_336.Location = New System.Drawing.Point(312, 648)
        Me._picFace_336.Name = "_picFace_336"
        Me._picFace_336.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_336.Size = New System.Drawing.Size(41, 41)
        Me._picFace_336.TabIndex = 350
        Me._picFace_336.TabStop = False
        '
        '_picFace_335
        '
        Me._picFace_335.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_335.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_335.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_335.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_335.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_335.Image = CType(resources.GetObject("_picFace_335.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_335, CType(335, Short))
        Me._picFace_335.Location = New System.Drawing.Point(352, 648)
        Me._picFace_335.Name = "_picFace_335"
        Me._picFace_335.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_335.Size = New System.Drawing.Size(41, 41)
        Me._picFace_335.TabIndex = 349
        Me._picFace_335.TabStop = False
        '
        '_picFace_334
        '
        Me._picFace_334.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_334.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_334.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_334.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_334.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_334.Image = CType(resources.GetObject("_picFace_334.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_334, CType(334, Short))
        Me._picFace_334.Location = New System.Drawing.Point(392, 648)
        Me._picFace_334.Name = "_picFace_334"
        Me._picFace_334.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_334.Size = New System.Drawing.Size(41, 41)
        Me._picFace_334.TabIndex = 348
        Me._picFace_334.TabStop = False
        '
        '_picFace_333
        '
        Me._picFace_333.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_333.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_333.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_333.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_333.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_333.Image = CType(resources.GetObject("_picFace_333.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_333, CType(333, Short))
        Me._picFace_333.Location = New System.Drawing.Point(432, 648)
        Me._picFace_333.Name = "_picFace_333"
        Me._picFace_333.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_333.Size = New System.Drawing.Size(41, 41)
        Me._picFace_333.TabIndex = 347
        Me._picFace_333.TabStop = False
        '
        '_picFace_332
        '
        Me._picFace_332.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_332.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_332.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_332.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_332.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_332.Image = CType(resources.GetObject("_picFace_332.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_332, CType(332, Short))
        Me._picFace_332.Location = New System.Drawing.Point(472, 648)
        Me._picFace_332.Name = "_picFace_332"
        Me._picFace_332.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_332.Size = New System.Drawing.Size(41, 41)
        Me._picFace_332.TabIndex = 346
        Me._picFace_332.TabStop = False
        '
        '_picFace_331
        '
        Me._picFace_331.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_331.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_331.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_331.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_331.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_331.Image = CType(resources.GetObject("_picFace_331.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_331, CType(331, Short))
        Me._picFace_331.Location = New System.Drawing.Point(512, 648)
        Me._picFace_331.Name = "_picFace_331"
        Me._picFace_331.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_331.Size = New System.Drawing.Size(41, 41)
        Me._picFace_331.TabIndex = 345
        Me._picFace_331.TabStop = False
        '
        '_picFace_330
        '
        Me._picFace_330.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_330.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_330.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_330.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_330.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_330.Image = CType(resources.GetObject("_picFace_330.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_330, CType(330, Short))
        Me._picFace_330.Location = New System.Drawing.Point(552, 648)
        Me._picFace_330.Name = "_picFace_330"
        Me._picFace_330.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_330.Size = New System.Drawing.Size(41, 41)
        Me._picFace_330.TabIndex = 344
        Me._picFace_330.TabStop = False
        '
        '_picFace_329
        '
        Me._picFace_329.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_329.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_329.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_329.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_329.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_329.Image = CType(resources.GetObject("_picFace_329.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_329, CType(329, Short))
        Me._picFace_329.Location = New System.Drawing.Point(592, 648)
        Me._picFace_329.Name = "_picFace_329"
        Me._picFace_329.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_329.Size = New System.Drawing.Size(41, 41)
        Me._picFace_329.TabIndex = 343
        Me._picFace_329.TabStop = False
        '
        '_picFace_328
        '
        Me._picFace_328.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_328.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_328.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_328.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_328.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_328.Image = CType(resources.GetObject("_picFace_328.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_328, CType(328, Short))
        Me._picFace_328.Location = New System.Drawing.Point(632, 648)
        Me._picFace_328.Name = "_picFace_328"
        Me._picFace_328.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_328.Size = New System.Drawing.Size(41, 41)
        Me._picFace_328.TabIndex = 342
        Me._picFace_328.TabStop = False
        '
        '_picFace_327
        '
        Me._picFace_327.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_327.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_327.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_327.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_327.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_327.Image = CType(resources.GetObject("_picFace_327.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_327, CType(327, Short))
        Me._picFace_327.Location = New System.Drawing.Point(672, 648)
        Me._picFace_327.Name = "_picFace_327"
        Me._picFace_327.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_327.Size = New System.Drawing.Size(41, 41)
        Me._picFace_327.TabIndex = 341
        Me._picFace_327.TabStop = False
        '
        '_picFace_326
        '
        Me._picFace_326.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_326.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_326.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_326.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_326.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_326.Image = CType(resources.GetObject("_picFace_326.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_326, CType(326, Short))
        Me._picFace_326.Location = New System.Drawing.Point(712, 648)
        Me._picFace_326.Name = "_picFace_326"
        Me._picFace_326.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_326.Size = New System.Drawing.Size(41, 41)
        Me._picFace_326.TabIndex = 340
        Me._picFace_326.TabStop = False
        '
        '_picFace_325
        '
        Me._picFace_325.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_325.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_325.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_325.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_325.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_325.Image = CType(resources.GetObject("_picFace_325.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_325, CType(325, Short))
        Me._picFace_325.Location = New System.Drawing.Point(752, 648)
        Me._picFace_325.Name = "_picFace_325"
        Me._picFace_325.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_325.Size = New System.Drawing.Size(41, 41)
        Me._picFace_325.TabIndex = 339
        Me._picFace_325.TabStop = False
        '
        '_picFace_324
        '
        Me._picFace_324.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_324.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_324.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_324.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_324.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_324.Image = CType(resources.GetObject("_picFace_324.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_324, CType(324, Short))
        Me._picFace_324.Location = New System.Drawing.Point(792, 648)
        Me._picFace_324.Name = "_picFace_324"
        Me._picFace_324.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_324.Size = New System.Drawing.Size(41, 41)
        Me._picFace_324.TabIndex = 338
        Me._picFace_324.TabStop = False
        '
        '_picFace_323
        '
        Me._picFace_323.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_323.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_323.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_323.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_323.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_323.Image = CType(resources.GetObject("_picFace_323.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_323, CType(323, Short))
        Me._picFace_323.Location = New System.Drawing.Point(832, 648)
        Me._picFace_323.Name = "_picFace_323"
        Me._picFace_323.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_323.Size = New System.Drawing.Size(41, 41)
        Me._picFace_323.TabIndex = 337
        Me._picFace_323.TabStop = False
        '
        '_picFace_322
        '
        Me._picFace_322.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_322.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_322.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_322.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_322.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_322.Image = CType(resources.GetObject("_picFace_322.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_322, CType(322, Short))
        Me._picFace_322.Location = New System.Drawing.Point(872, 648)
        Me._picFace_322.Name = "_picFace_322"
        Me._picFace_322.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_322.Size = New System.Drawing.Size(41, 41)
        Me._picFace_322.TabIndex = 336
        Me._picFace_322.TabStop = False
        '
        '_picFace_321
        '
        Me._picFace_321.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_321.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_321.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_321.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_321.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_321.Image = CType(resources.GetObject("_picFace_321.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_321, CType(321, Short))
        Me._picFace_321.Location = New System.Drawing.Point(912, 648)
        Me._picFace_321.Name = "_picFace_321"
        Me._picFace_321.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_321.Size = New System.Drawing.Size(41, 41)
        Me._picFace_321.TabIndex = 335
        Me._picFace_321.TabStop = False
        '
        '_picFace_320
        '
        Me._picFace_320.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_320.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_320.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_320.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_320.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_320.Image = CType(resources.GetObject("_picFace_320.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_320, CType(320, Short))
        Me._picFace_320.Location = New System.Drawing.Point(952, 648)
        Me._picFace_320.Name = "_picFace_320"
        Me._picFace_320.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_320.Size = New System.Drawing.Size(41, 41)
        Me._picFace_320.TabIndex = 334
        Me._picFace_320.TabStop = False
        '
        '_picFace_319
        '
        Me._picFace_319.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_319.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_319.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_319.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_319.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_319.Image = CType(resources.GetObject("_picFace_319.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_319, CType(319, Short))
        Me._picFace_319.Location = New System.Drawing.Point(192, 608)
        Me._picFace_319.Name = "_picFace_319"
        Me._picFace_319.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_319.Size = New System.Drawing.Size(41, 41)
        Me._picFace_319.TabIndex = 333
        Me._picFace_319.TabStop = False
        '
        '_picFace_318
        '
        Me._picFace_318.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_318.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_318.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_318.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_318.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_318.Image = CType(resources.GetObject("_picFace_318.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_318, CType(318, Short))
        Me._picFace_318.Location = New System.Drawing.Point(232, 608)
        Me._picFace_318.Name = "_picFace_318"
        Me._picFace_318.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_318.Size = New System.Drawing.Size(41, 41)
        Me._picFace_318.TabIndex = 332
        Me._picFace_318.TabStop = False
        '
        '_picFace_317
        '
        Me._picFace_317.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_317.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_317.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_317.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_317.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_317.Image = CType(resources.GetObject("_picFace_317.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_317, CType(317, Short))
        Me._picFace_317.Location = New System.Drawing.Point(272, 608)
        Me._picFace_317.Name = "_picFace_317"
        Me._picFace_317.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_317.Size = New System.Drawing.Size(41, 41)
        Me._picFace_317.TabIndex = 331
        Me._picFace_317.TabStop = False
        '
        '_picFace_316
        '
        Me._picFace_316.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_316.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_316.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_316.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_316.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_316.Image = CType(resources.GetObject("_picFace_316.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_316, CType(316, Short))
        Me._picFace_316.Location = New System.Drawing.Point(312, 608)
        Me._picFace_316.Name = "_picFace_316"
        Me._picFace_316.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_316.Size = New System.Drawing.Size(41, 41)
        Me._picFace_316.TabIndex = 330
        Me._picFace_316.TabStop = False
        '
        '_picFace_315
        '
        Me._picFace_315.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_315.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_315.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_315.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_315.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_315.Image = CType(resources.GetObject("_picFace_315.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_315, CType(315, Short))
        Me._picFace_315.Location = New System.Drawing.Point(352, 608)
        Me._picFace_315.Name = "_picFace_315"
        Me._picFace_315.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_315.Size = New System.Drawing.Size(41, 41)
        Me._picFace_315.TabIndex = 329
        Me._picFace_315.TabStop = False
        '
        '_picFace_314
        '
        Me._picFace_314.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_314.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_314.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_314.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_314.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_314.Image = CType(resources.GetObject("_picFace_314.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_314, CType(314, Short))
        Me._picFace_314.Location = New System.Drawing.Point(392, 608)
        Me._picFace_314.Name = "_picFace_314"
        Me._picFace_314.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_314.Size = New System.Drawing.Size(41, 41)
        Me._picFace_314.TabIndex = 328
        Me._picFace_314.TabStop = False
        '
        '_picFace_313
        '
        Me._picFace_313.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_313.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_313.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_313.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_313.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_313.Image = CType(resources.GetObject("_picFace_313.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_313, CType(313, Short))
        Me._picFace_313.Location = New System.Drawing.Point(432, 608)
        Me._picFace_313.Name = "_picFace_313"
        Me._picFace_313.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_313.Size = New System.Drawing.Size(41, 41)
        Me._picFace_313.TabIndex = 327
        Me._picFace_313.TabStop = False
        '
        '_picFace_312
        '
        Me._picFace_312.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_312.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_312.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_312.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_312.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_312.Image = CType(resources.GetObject("_picFace_312.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_312, CType(312, Short))
        Me._picFace_312.Location = New System.Drawing.Point(472, 608)
        Me._picFace_312.Name = "_picFace_312"
        Me._picFace_312.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_312.Size = New System.Drawing.Size(41, 41)
        Me._picFace_312.TabIndex = 326
        Me._picFace_312.TabStop = False
        '
        '_picFace_311
        '
        Me._picFace_311.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_311.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_311.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_311.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_311.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_311.Image = CType(resources.GetObject("_picFace_311.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_311, CType(311, Short))
        Me._picFace_311.Location = New System.Drawing.Point(512, 608)
        Me._picFace_311.Name = "_picFace_311"
        Me._picFace_311.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_311.Size = New System.Drawing.Size(41, 41)
        Me._picFace_311.TabIndex = 325
        Me._picFace_311.TabStop = False
        '
        '_picFace_310
        '
        Me._picFace_310.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_310.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_310.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_310.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_310.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_310.Image = CType(resources.GetObject("_picFace_310.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_310, CType(310, Short))
        Me._picFace_310.Location = New System.Drawing.Point(552, 608)
        Me._picFace_310.Name = "_picFace_310"
        Me._picFace_310.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_310.Size = New System.Drawing.Size(41, 41)
        Me._picFace_310.TabIndex = 324
        Me._picFace_310.TabStop = False
        '
        '_picFace_309
        '
        Me._picFace_309.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_309.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_309.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_309.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_309.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_309.Image = CType(resources.GetObject("_picFace_309.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_309, CType(309, Short))
        Me._picFace_309.Location = New System.Drawing.Point(592, 608)
        Me._picFace_309.Name = "_picFace_309"
        Me._picFace_309.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_309.Size = New System.Drawing.Size(41, 41)
        Me._picFace_309.TabIndex = 323
        Me._picFace_309.TabStop = False
        '
        '_picFace_308
        '
        Me._picFace_308.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_308.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_308.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_308.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_308.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_308.Image = CType(resources.GetObject("_picFace_308.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_308, CType(308, Short))
        Me._picFace_308.Location = New System.Drawing.Point(632, 608)
        Me._picFace_308.Name = "_picFace_308"
        Me._picFace_308.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_308.Size = New System.Drawing.Size(41, 41)
        Me._picFace_308.TabIndex = 322
        Me._picFace_308.TabStop = False
        '
        '_picFace_307
        '
        Me._picFace_307.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_307.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_307.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_307.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_307.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_307.Image = CType(resources.GetObject("_picFace_307.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_307, CType(307, Short))
        Me._picFace_307.Location = New System.Drawing.Point(672, 608)
        Me._picFace_307.Name = "_picFace_307"
        Me._picFace_307.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_307.Size = New System.Drawing.Size(41, 41)
        Me._picFace_307.TabIndex = 321
        Me._picFace_307.TabStop = False
        '
        '_picFace_306
        '
        Me._picFace_306.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_306.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_306.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_306.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_306.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_306.Image = CType(resources.GetObject("_picFace_306.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_306, CType(306, Short))
        Me._picFace_306.Location = New System.Drawing.Point(712, 608)
        Me._picFace_306.Name = "_picFace_306"
        Me._picFace_306.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_306.Size = New System.Drawing.Size(41, 41)
        Me._picFace_306.TabIndex = 320
        Me._picFace_306.TabStop = False
        '
        '_picFace_305
        '
        Me._picFace_305.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_305.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_305.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_305.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_305.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_305.Image = CType(resources.GetObject("_picFace_305.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_305, CType(305, Short))
        Me._picFace_305.Location = New System.Drawing.Point(752, 608)
        Me._picFace_305.Name = "_picFace_305"
        Me._picFace_305.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_305.Size = New System.Drawing.Size(41, 41)
        Me._picFace_305.TabIndex = 319
        Me._picFace_305.TabStop = False
        '
        '_picFace_304
        '
        Me._picFace_304.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_304.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_304.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_304.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_304.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_304.Image = CType(resources.GetObject("_picFace_304.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_304, CType(304, Short))
        Me._picFace_304.Location = New System.Drawing.Point(792, 608)
        Me._picFace_304.Name = "_picFace_304"
        Me._picFace_304.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_304.Size = New System.Drawing.Size(41, 41)
        Me._picFace_304.TabIndex = 318
        Me._picFace_304.TabStop = False
        '
        '_picFace_303
        '
        Me._picFace_303.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_303.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_303.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_303.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_303.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_303.Image = CType(resources.GetObject("_picFace_303.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_303, CType(303, Short))
        Me._picFace_303.Location = New System.Drawing.Point(832, 608)
        Me._picFace_303.Name = "_picFace_303"
        Me._picFace_303.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_303.Size = New System.Drawing.Size(41, 41)
        Me._picFace_303.TabIndex = 317
        Me._picFace_303.TabStop = False
        '
        '_picFace_302
        '
        Me._picFace_302.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_302.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_302.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_302.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_302.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_302.Image = CType(resources.GetObject("_picFace_302.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_302, CType(302, Short))
        Me._picFace_302.Location = New System.Drawing.Point(872, 608)
        Me._picFace_302.Name = "_picFace_302"
        Me._picFace_302.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_302.Size = New System.Drawing.Size(41, 41)
        Me._picFace_302.TabIndex = 316
        Me._picFace_302.TabStop = False
        '
        '_picFace_301
        '
        Me._picFace_301.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_301.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_301.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_301.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_301.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_301.Image = CType(resources.GetObject("_picFace_301.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_301, CType(301, Short))
        Me._picFace_301.Location = New System.Drawing.Point(912, 608)
        Me._picFace_301.Name = "_picFace_301"
        Me._picFace_301.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_301.Size = New System.Drawing.Size(41, 41)
        Me._picFace_301.TabIndex = 315
        Me._picFace_301.TabStop = False
        '
        '_picFace_300
        '
        Me._picFace_300.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_300.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_300.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_300.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_300.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_300.Image = CType(resources.GetObject("_picFace_300.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_300, CType(300, Short))
        Me._picFace_300.Location = New System.Drawing.Point(952, 608)
        Me._picFace_300.Name = "_picFace_300"
        Me._picFace_300.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_300.Size = New System.Drawing.Size(41, 41)
        Me._picFace_300.TabIndex = 314
        Me._picFace_300.TabStop = False
        '
        '_picFace_299
        '
        Me._picFace_299.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_299.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_299.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_299.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_299.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_299.Image = CType(resources.GetObject("_picFace_299.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_299, CType(299, Short))
        Me._picFace_299.Location = New System.Drawing.Point(192, 568)
        Me._picFace_299.Name = "_picFace_299"
        Me._picFace_299.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_299.Size = New System.Drawing.Size(41, 41)
        Me._picFace_299.TabIndex = 313
        Me._picFace_299.TabStop = False
        '
        '_picFace_298
        '
        Me._picFace_298.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_298.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_298.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_298.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_298.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_298.Image = CType(resources.GetObject("_picFace_298.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_298, CType(298, Short))
        Me._picFace_298.Location = New System.Drawing.Point(232, 568)
        Me._picFace_298.Name = "_picFace_298"
        Me._picFace_298.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_298.Size = New System.Drawing.Size(41, 41)
        Me._picFace_298.TabIndex = 312
        Me._picFace_298.TabStop = False
        '
        '_picFace_297
        '
        Me._picFace_297.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_297.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_297.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_297.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_297.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_297.Image = CType(resources.GetObject("_picFace_297.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_297, CType(297, Short))
        Me._picFace_297.Location = New System.Drawing.Point(272, 568)
        Me._picFace_297.Name = "_picFace_297"
        Me._picFace_297.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_297.Size = New System.Drawing.Size(41, 41)
        Me._picFace_297.TabIndex = 311
        Me._picFace_297.TabStop = False
        '
        '_picFace_296
        '
        Me._picFace_296.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_296.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_296.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_296.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_296.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_296.Image = CType(resources.GetObject("_picFace_296.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_296, CType(296, Short))
        Me._picFace_296.Location = New System.Drawing.Point(312, 568)
        Me._picFace_296.Name = "_picFace_296"
        Me._picFace_296.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_296.Size = New System.Drawing.Size(41, 41)
        Me._picFace_296.TabIndex = 310
        Me._picFace_296.TabStop = False
        '
        '_picFace_295
        '
        Me._picFace_295.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_295.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_295.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_295.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_295.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_295.Image = CType(resources.GetObject("_picFace_295.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_295, CType(295, Short))
        Me._picFace_295.Location = New System.Drawing.Point(352, 568)
        Me._picFace_295.Name = "_picFace_295"
        Me._picFace_295.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_295.Size = New System.Drawing.Size(41, 41)
        Me._picFace_295.TabIndex = 309
        Me._picFace_295.TabStop = False
        '
        '_picFace_294
        '
        Me._picFace_294.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_294.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_294.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_294.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_294.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_294.Image = CType(resources.GetObject("_picFace_294.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_294, CType(294, Short))
        Me._picFace_294.Location = New System.Drawing.Point(392, 568)
        Me._picFace_294.Name = "_picFace_294"
        Me._picFace_294.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_294.Size = New System.Drawing.Size(41, 41)
        Me._picFace_294.TabIndex = 308
        Me._picFace_294.TabStop = False
        '
        '_picFace_293
        '
        Me._picFace_293.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_293.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_293.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_293.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_293.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_293.Image = CType(resources.GetObject("_picFace_293.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_293, CType(293, Short))
        Me._picFace_293.Location = New System.Drawing.Point(432, 568)
        Me._picFace_293.Name = "_picFace_293"
        Me._picFace_293.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_293.Size = New System.Drawing.Size(41, 41)
        Me._picFace_293.TabIndex = 307
        Me._picFace_293.TabStop = False
        '
        '_picFace_292
        '
        Me._picFace_292.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_292.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_292.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_292.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_292.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_292.Image = CType(resources.GetObject("_picFace_292.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_292, CType(292, Short))
        Me._picFace_292.Location = New System.Drawing.Point(472, 568)
        Me._picFace_292.Name = "_picFace_292"
        Me._picFace_292.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_292.Size = New System.Drawing.Size(41, 41)
        Me._picFace_292.TabIndex = 306
        Me._picFace_292.TabStop = False
        '
        '_picFace_291
        '
        Me._picFace_291.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_291.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_291.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_291.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_291.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_291.Image = CType(resources.GetObject("_picFace_291.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_291, CType(291, Short))
        Me._picFace_291.Location = New System.Drawing.Point(512, 568)
        Me._picFace_291.Name = "_picFace_291"
        Me._picFace_291.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_291.Size = New System.Drawing.Size(41, 41)
        Me._picFace_291.TabIndex = 305
        Me._picFace_291.TabStop = False
        '
        '_picFace_290
        '
        Me._picFace_290.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_290.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_290.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_290.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_290.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_290.Image = CType(resources.GetObject("_picFace_290.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_290, CType(290, Short))
        Me._picFace_290.Location = New System.Drawing.Point(552, 568)
        Me._picFace_290.Name = "_picFace_290"
        Me._picFace_290.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_290.Size = New System.Drawing.Size(41, 41)
        Me._picFace_290.TabIndex = 304
        Me._picFace_290.TabStop = False
        '
        '_picFace_289
        '
        Me._picFace_289.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_289.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_289.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_289.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_289.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_289.Image = CType(resources.GetObject("_picFace_289.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_289, CType(289, Short))
        Me._picFace_289.Location = New System.Drawing.Point(592, 568)
        Me._picFace_289.Name = "_picFace_289"
        Me._picFace_289.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_289.Size = New System.Drawing.Size(41, 41)
        Me._picFace_289.TabIndex = 303
        Me._picFace_289.TabStop = False
        '
        '_picFace_288
        '
        Me._picFace_288.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_288.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_288.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_288.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_288.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_288.Image = CType(resources.GetObject("_picFace_288.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_288, CType(288, Short))
        Me._picFace_288.Location = New System.Drawing.Point(632, 568)
        Me._picFace_288.Name = "_picFace_288"
        Me._picFace_288.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_288.Size = New System.Drawing.Size(41, 41)
        Me._picFace_288.TabIndex = 302
        Me._picFace_288.TabStop = False
        '
        '_picFace_287
        '
        Me._picFace_287.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_287.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_287.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_287.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_287.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_287.Image = CType(resources.GetObject("_picFace_287.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_287, CType(287, Short))
        Me._picFace_287.Location = New System.Drawing.Point(672, 568)
        Me._picFace_287.Name = "_picFace_287"
        Me._picFace_287.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_287.Size = New System.Drawing.Size(41, 41)
        Me._picFace_287.TabIndex = 301
        Me._picFace_287.TabStop = False
        '
        '_picFace_286
        '
        Me._picFace_286.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_286.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_286.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_286.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_286.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_286.Image = CType(resources.GetObject("_picFace_286.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_286, CType(286, Short))
        Me._picFace_286.Location = New System.Drawing.Point(712, 568)
        Me._picFace_286.Name = "_picFace_286"
        Me._picFace_286.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_286.Size = New System.Drawing.Size(41, 41)
        Me._picFace_286.TabIndex = 300
        Me._picFace_286.TabStop = False
        '
        '_picFace_285
        '
        Me._picFace_285.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_285.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_285.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_285.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_285.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_285.Image = CType(resources.GetObject("_picFace_285.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_285, CType(285, Short))
        Me._picFace_285.Location = New System.Drawing.Point(752, 568)
        Me._picFace_285.Name = "_picFace_285"
        Me._picFace_285.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_285.Size = New System.Drawing.Size(41, 41)
        Me._picFace_285.TabIndex = 299
        Me._picFace_285.TabStop = False
        '
        '_picFace_284
        '
        Me._picFace_284.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_284.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_284.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_284.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_284.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_284.Image = CType(resources.GetObject("_picFace_284.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_284, CType(284, Short))
        Me._picFace_284.Location = New System.Drawing.Point(792, 568)
        Me._picFace_284.Name = "_picFace_284"
        Me._picFace_284.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_284.Size = New System.Drawing.Size(41, 41)
        Me._picFace_284.TabIndex = 298
        Me._picFace_284.TabStop = False
        '
        '_picFace_283
        '
        Me._picFace_283.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_283.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_283.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_283.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_283.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_283.Image = CType(resources.GetObject("_picFace_283.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_283, CType(283, Short))
        Me._picFace_283.Location = New System.Drawing.Point(832, 568)
        Me._picFace_283.Name = "_picFace_283"
        Me._picFace_283.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_283.Size = New System.Drawing.Size(41, 41)
        Me._picFace_283.TabIndex = 297
        Me._picFace_283.TabStop = False
        '
        '_picFace_282
        '
        Me._picFace_282.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_282.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_282.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_282.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_282.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_282.Image = CType(resources.GetObject("_picFace_282.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_282, CType(282, Short))
        Me._picFace_282.Location = New System.Drawing.Point(872, 568)
        Me._picFace_282.Name = "_picFace_282"
        Me._picFace_282.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_282.Size = New System.Drawing.Size(41, 41)
        Me._picFace_282.TabIndex = 296
        Me._picFace_282.TabStop = False
        '
        '_picFace_281
        '
        Me._picFace_281.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_281.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_281.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_281.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_281.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_281.Image = CType(resources.GetObject("_picFace_281.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_281, CType(281, Short))
        Me._picFace_281.Location = New System.Drawing.Point(912, 568)
        Me._picFace_281.Name = "_picFace_281"
        Me._picFace_281.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_281.Size = New System.Drawing.Size(41, 41)
        Me._picFace_281.TabIndex = 295
        Me._picFace_281.TabStop = False
        '
        '_picFace_280
        '
        Me._picFace_280.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_280.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_280.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_280.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_280.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_280.Image = CType(resources.GetObject("_picFace_280.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_280, CType(280, Short))
        Me._picFace_280.Location = New System.Drawing.Point(952, 568)
        Me._picFace_280.Name = "_picFace_280"
        Me._picFace_280.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_280.Size = New System.Drawing.Size(41, 41)
        Me._picFace_280.TabIndex = 294
        Me._picFace_280.TabStop = False
        '
        '_picFace_279
        '
        Me._picFace_279.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_279.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_279.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_279.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_279.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_279.Image = CType(resources.GetObject("_picFace_279.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_279, CType(279, Short))
        Me._picFace_279.Location = New System.Drawing.Point(192, 528)
        Me._picFace_279.Name = "_picFace_279"
        Me._picFace_279.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_279.Size = New System.Drawing.Size(41, 41)
        Me._picFace_279.TabIndex = 293
        Me._picFace_279.TabStop = False
        '
        '_picFace_278
        '
        Me._picFace_278.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_278.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_278.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_278.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_278.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_278.Image = CType(resources.GetObject("_picFace_278.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_278, CType(278, Short))
        Me._picFace_278.Location = New System.Drawing.Point(232, 528)
        Me._picFace_278.Name = "_picFace_278"
        Me._picFace_278.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_278.Size = New System.Drawing.Size(41, 41)
        Me._picFace_278.TabIndex = 292
        Me._picFace_278.TabStop = False
        '
        '_picFace_277
        '
        Me._picFace_277.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_277.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_277.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_277.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_277.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_277.Image = CType(resources.GetObject("_picFace_277.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_277, CType(277, Short))
        Me._picFace_277.Location = New System.Drawing.Point(272, 528)
        Me._picFace_277.Name = "_picFace_277"
        Me._picFace_277.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_277.Size = New System.Drawing.Size(41, 41)
        Me._picFace_277.TabIndex = 291
        Me._picFace_277.TabStop = False
        '
        '_picFace_276
        '
        Me._picFace_276.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_276.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_276.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_276.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_276.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_276.Image = CType(resources.GetObject("_picFace_276.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_276, CType(276, Short))
        Me._picFace_276.Location = New System.Drawing.Point(312, 528)
        Me._picFace_276.Name = "_picFace_276"
        Me._picFace_276.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_276.Size = New System.Drawing.Size(41, 41)
        Me._picFace_276.TabIndex = 290
        Me._picFace_276.TabStop = False
        '
        '_picFace_275
        '
        Me._picFace_275.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_275.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_275.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_275.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_275.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_275.Image = CType(resources.GetObject("_picFace_275.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_275, CType(275, Short))
        Me._picFace_275.Location = New System.Drawing.Point(352, 528)
        Me._picFace_275.Name = "_picFace_275"
        Me._picFace_275.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_275.Size = New System.Drawing.Size(41, 41)
        Me._picFace_275.TabIndex = 289
        Me._picFace_275.TabStop = False
        '
        '_picFace_274
        '
        Me._picFace_274.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_274.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_274.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_274.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_274.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_274.Image = CType(resources.GetObject("_picFace_274.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_274, CType(274, Short))
        Me._picFace_274.Location = New System.Drawing.Point(392, 528)
        Me._picFace_274.Name = "_picFace_274"
        Me._picFace_274.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_274.Size = New System.Drawing.Size(41, 41)
        Me._picFace_274.TabIndex = 288
        Me._picFace_274.TabStop = False
        '
        '_picFace_273
        '
        Me._picFace_273.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_273.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_273.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_273.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_273.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_273.Image = CType(resources.GetObject("_picFace_273.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_273, CType(273, Short))
        Me._picFace_273.Location = New System.Drawing.Point(432, 528)
        Me._picFace_273.Name = "_picFace_273"
        Me._picFace_273.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_273.Size = New System.Drawing.Size(41, 41)
        Me._picFace_273.TabIndex = 287
        Me._picFace_273.TabStop = False
        '
        '_picFace_272
        '
        Me._picFace_272.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_272.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_272.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_272.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_272.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_272.Image = CType(resources.GetObject("_picFace_272.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_272, CType(272, Short))
        Me._picFace_272.Location = New System.Drawing.Point(472, 528)
        Me._picFace_272.Name = "_picFace_272"
        Me._picFace_272.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_272.Size = New System.Drawing.Size(41, 41)
        Me._picFace_272.TabIndex = 286
        Me._picFace_272.TabStop = False
        '
        '_picFace_271
        '
        Me._picFace_271.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_271.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_271.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_271.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_271.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_271.Image = CType(resources.GetObject("_picFace_271.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_271, CType(271, Short))
        Me._picFace_271.Location = New System.Drawing.Point(512, 528)
        Me._picFace_271.Name = "_picFace_271"
        Me._picFace_271.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_271.Size = New System.Drawing.Size(41, 41)
        Me._picFace_271.TabIndex = 285
        Me._picFace_271.TabStop = False
        '
        '_picFace_270
        '
        Me._picFace_270.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_270.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_270.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_270.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_270.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_270.Image = CType(resources.GetObject("_picFace_270.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_270, CType(270, Short))
        Me._picFace_270.Location = New System.Drawing.Point(552, 528)
        Me._picFace_270.Name = "_picFace_270"
        Me._picFace_270.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_270.Size = New System.Drawing.Size(41, 41)
        Me._picFace_270.TabIndex = 284
        Me._picFace_270.TabStop = False
        '
        '_picFace_269
        '
        Me._picFace_269.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_269.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_269.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_269.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_269.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_269.Image = CType(resources.GetObject("_picFace_269.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_269, CType(269, Short))
        Me._picFace_269.Location = New System.Drawing.Point(592, 528)
        Me._picFace_269.Name = "_picFace_269"
        Me._picFace_269.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_269.Size = New System.Drawing.Size(41, 41)
        Me._picFace_269.TabIndex = 283
        Me._picFace_269.TabStop = False
        '
        '_picFace_268
        '
        Me._picFace_268.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_268.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_268.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_268.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_268.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_268.Image = CType(resources.GetObject("_picFace_268.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_268, CType(268, Short))
        Me._picFace_268.Location = New System.Drawing.Point(632, 528)
        Me._picFace_268.Name = "_picFace_268"
        Me._picFace_268.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_268.Size = New System.Drawing.Size(41, 41)
        Me._picFace_268.TabIndex = 282
        Me._picFace_268.TabStop = False
        '
        '_picFace_267
        '
        Me._picFace_267.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_267.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_267.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_267.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_267.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_267.Image = CType(resources.GetObject("_picFace_267.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_267, CType(267, Short))
        Me._picFace_267.Location = New System.Drawing.Point(672, 528)
        Me._picFace_267.Name = "_picFace_267"
        Me._picFace_267.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_267.Size = New System.Drawing.Size(41, 41)
        Me._picFace_267.TabIndex = 281
        Me._picFace_267.TabStop = False
        '
        '_picFace_266
        '
        Me._picFace_266.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_266.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_266.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_266.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_266.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_266.Image = CType(resources.GetObject("_picFace_266.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_266, CType(266, Short))
        Me._picFace_266.Location = New System.Drawing.Point(712, 528)
        Me._picFace_266.Name = "_picFace_266"
        Me._picFace_266.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_266.Size = New System.Drawing.Size(41, 41)
        Me._picFace_266.TabIndex = 280
        Me._picFace_266.TabStop = False
        '
        '_picFace_265
        '
        Me._picFace_265.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_265.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_265.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_265.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_265.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_265.Image = CType(resources.GetObject("_picFace_265.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_265, CType(265, Short))
        Me._picFace_265.Location = New System.Drawing.Point(752, 528)
        Me._picFace_265.Name = "_picFace_265"
        Me._picFace_265.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_265.Size = New System.Drawing.Size(41, 41)
        Me._picFace_265.TabIndex = 279
        Me._picFace_265.TabStop = False
        '
        '_picFace_264
        '
        Me._picFace_264.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_264.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_264.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_264.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_264.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_264.Image = CType(resources.GetObject("_picFace_264.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_264, CType(264, Short))
        Me._picFace_264.Location = New System.Drawing.Point(792, 528)
        Me._picFace_264.Name = "_picFace_264"
        Me._picFace_264.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_264.Size = New System.Drawing.Size(41, 41)
        Me._picFace_264.TabIndex = 278
        Me._picFace_264.TabStop = False
        '
        '_picFace_263
        '
        Me._picFace_263.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_263.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_263.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_263.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_263.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_263.Image = CType(resources.GetObject("_picFace_263.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_263, CType(263, Short))
        Me._picFace_263.Location = New System.Drawing.Point(832, 528)
        Me._picFace_263.Name = "_picFace_263"
        Me._picFace_263.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_263.Size = New System.Drawing.Size(41, 41)
        Me._picFace_263.TabIndex = 277
        Me._picFace_263.TabStop = False
        '
        '_picFace_262
        '
        Me._picFace_262.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_262.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_262.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_262.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_262.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_262.Image = CType(resources.GetObject("_picFace_262.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_262, CType(262, Short))
        Me._picFace_262.Location = New System.Drawing.Point(872, 528)
        Me._picFace_262.Name = "_picFace_262"
        Me._picFace_262.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_262.Size = New System.Drawing.Size(41, 41)
        Me._picFace_262.TabIndex = 276
        Me._picFace_262.TabStop = False
        '
        '_picFace_261
        '
        Me._picFace_261.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_261.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_261.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_261.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_261.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_261.Image = CType(resources.GetObject("_picFace_261.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_261, CType(261, Short))
        Me._picFace_261.Location = New System.Drawing.Point(912, 528)
        Me._picFace_261.Name = "_picFace_261"
        Me._picFace_261.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_261.Size = New System.Drawing.Size(41, 41)
        Me._picFace_261.TabIndex = 275
        Me._picFace_261.TabStop = False
        '
        '_picFace_260
        '
        Me._picFace_260.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_260.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_260.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_260.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_260.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_260.Image = CType(resources.GetObject("_picFace_260.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_260, CType(260, Short))
        Me._picFace_260.Location = New System.Drawing.Point(952, 528)
        Me._picFace_260.Name = "_picFace_260"
        Me._picFace_260.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_260.Size = New System.Drawing.Size(41, 41)
        Me._picFace_260.TabIndex = 274
        Me._picFace_260.TabStop = False
        '
        '_picFace_259
        '
        Me._picFace_259.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_259.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_259.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_259.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_259.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_259.Image = CType(resources.GetObject("_picFace_259.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_259, CType(259, Short))
        Me._picFace_259.Location = New System.Drawing.Point(192, 488)
        Me._picFace_259.Name = "_picFace_259"
        Me._picFace_259.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_259.Size = New System.Drawing.Size(41, 41)
        Me._picFace_259.TabIndex = 273
        Me._picFace_259.TabStop = False
        '
        '_picFace_258
        '
        Me._picFace_258.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_258.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_258.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_258.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_258.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_258.Image = CType(resources.GetObject("_picFace_258.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_258, CType(258, Short))
        Me._picFace_258.Location = New System.Drawing.Point(232, 488)
        Me._picFace_258.Name = "_picFace_258"
        Me._picFace_258.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_258.Size = New System.Drawing.Size(41, 41)
        Me._picFace_258.TabIndex = 272
        Me._picFace_258.TabStop = False
        '
        '_picFace_257
        '
        Me._picFace_257.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_257.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_257.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_257.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_257.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_257.Image = CType(resources.GetObject("_picFace_257.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_257, CType(257, Short))
        Me._picFace_257.Location = New System.Drawing.Point(272, 488)
        Me._picFace_257.Name = "_picFace_257"
        Me._picFace_257.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_257.Size = New System.Drawing.Size(41, 41)
        Me._picFace_257.TabIndex = 271
        Me._picFace_257.TabStop = False
        '
        '_picFace_256
        '
        Me._picFace_256.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_256.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_256.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_256.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_256.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_256.Image = CType(resources.GetObject("_picFace_256.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_256, CType(256, Short))
        Me._picFace_256.Location = New System.Drawing.Point(312, 488)
        Me._picFace_256.Name = "_picFace_256"
        Me._picFace_256.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_256.Size = New System.Drawing.Size(41, 41)
        Me._picFace_256.TabIndex = 270
        Me._picFace_256.TabStop = False
        '
        '_picFace_255
        '
        Me._picFace_255.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_255.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_255.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_255.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_255.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_255.Image = CType(resources.GetObject("_picFace_255.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_255, CType(255, Short))
        Me._picFace_255.Location = New System.Drawing.Point(352, 488)
        Me._picFace_255.Name = "_picFace_255"
        Me._picFace_255.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_255.Size = New System.Drawing.Size(41, 41)
        Me._picFace_255.TabIndex = 269
        Me._picFace_255.TabStop = False
        '
        '_picFace_254
        '
        Me._picFace_254.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_254.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_254.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_254.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_254.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_254.Image = CType(resources.GetObject("_picFace_254.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_254, CType(254, Short))
        Me._picFace_254.Location = New System.Drawing.Point(392, 488)
        Me._picFace_254.Name = "_picFace_254"
        Me._picFace_254.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_254.Size = New System.Drawing.Size(41, 41)
        Me._picFace_254.TabIndex = 268
        Me._picFace_254.TabStop = False
        '
        '_picFace_253
        '
        Me._picFace_253.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_253.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_253.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_253.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_253.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_253.Image = CType(resources.GetObject("_picFace_253.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_253, CType(253, Short))
        Me._picFace_253.Location = New System.Drawing.Point(432, 488)
        Me._picFace_253.Name = "_picFace_253"
        Me._picFace_253.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_253.Size = New System.Drawing.Size(41, 41)
        Me._picFace_253.TabIndex = 267
        Me._picFace_253.TabStop = False
        '
        '_picFace_252
        '
        Me._picFace_252.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_252.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_252.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_252.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_252.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_252.Image = CType(resources.GetObject("_picFace_252.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_252, CType(252, Short))
        Me._picFace_252.Location = New System.Drawing.Point(472, 488)
        Me._picFace_252.Name = "_picFace_252"
        Me._picFace_252.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_252.Size = New System.Drawing.Size(41, 41)
        Me._picFace_252.TabIndex = 266
        Me._picFace_252.TabStop = False
        '
        '_picFace_251
        '
        Me._picFace_251.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_251.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_251.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_251.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_251.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_251.Image = CType(resources.GetObject("_picFace_251.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_251, CType(251, Short))
        Me._picFace_251.Location = New System.Drawing.Point(512, 488)
        Me._picFace_251.Name = "_picFace_251"
        Me._picFace_251.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_251.Size = New System.Drawing.Size(41, 41)
        Me._picFace_251.TabIndex = 265
        Me._picFace_251.TabStop = False
        '
        '_picFace_250
        '
        Me._picFace_250.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_250.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_250.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_250.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_250.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_250.Image = CType(resources.GetObject("_picFace_250.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_250, CType(250, Short))
        Me._picFace_250.Location = New System.Drawing.Point(552, 488)
        Me._picFace_250.Name = "_picFace_250"
        Me._picFace_250.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_250.Size = New System.Drawing.Size(41, 41)
        Me._picFace_250.TabIndex = 264
        Me._picFace_250.TabStop = False
        '
        '_picFace_249
        '
        Me._picFace_249.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_249.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_249.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_249.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_249.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_249.Image = CType(resources.GetObject("_picFace_249.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_249, CType(249, Short))
        Me._picFace_249.Location = New System.Drawing.Point(592, 488)
        Me._picFace_249.Name = "_picFace_249"
        Me._picFace_249.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_249.Size = New System.Drawing.Size(41, 41)
        Me._picFace_249.TabIndex = 263
        Me._picFace_249.TabStop = False
        '
        '_picFace_248
        '
        Me._picFace_248.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_248.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_248.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_248.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_248.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_248.Image = CType(resources.GetObject("_picFace_248.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_248, CType(248, Short))
        Me._picFace_248.Location = New System.Drawing.Point(632, 488)
        Me._picFace_248.Name = "_picFace_248"
        Me._picFace_248.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_248.Size = New System.Drawing.Size(41, 41)
        Me._picFace_248.TabIndex = 262
        Me._picFace_248.TabStop = False
        '
        '_picFace_247
        '
        Me._picFace_247.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_247.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_247.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_247.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_247.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_247.Image = CType(resources.GetObject("_picFace_247.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_247, CType(247, Short))
        Me._picFace_247.Location = New System.Drawing.Point(672, 488)
        Me._picFace_247.Name = "_picFace_247"
        Me._picFace_247.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_247.Size = New System.Drawing.Size(41, 41)
        Me._picFace_247.TabIndex = 261
        Me._picFace_247.TabStop = False
        '
        '_picFace_246
        '
        Me._picFace_246.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_246.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_246.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_246.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_246.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_246.Image = CType(resources.GetObject("_picFace_246.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_246, CType(246, Short))
        Me._picFace_246.Location = New System.Drawing.Point(712, 488)
        Me._picFace_246.Name = "_picFace_246"
        Me._picFace_246.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_246.Size = New System.Drawing.Size(41, 41)
        Me._picFace_246.TabIndex = 260
        Me._picFace_246.TabStop = False
        '
        '_picFace_245
        '
        Me._picFace_245.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_245.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_245.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_245.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_245.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_245.Image = CType(resources.GetObject("_picFace_245.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_245, CType(245, Short))
        Me._picFace_245.Location = New System.Drawing.Point(752, 488)
        Me._picFace_245.Name = "_picFace_245"
        Me._picFace_245.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_245.Size = New System.Drawing.Size(41, 41)
        Me._picFace_245.TabIndex = 259
        Me._picFace_245.TabStop = False
        '
        '_picFace_244
        '
        Me._picFace_244.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_244.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_244.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_244.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_244.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_244.Image = CType(resources.GetObject("_picFace_244.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_244, CType(244, Short))
        Me._picFace_244.Location = New System.Drawing.Point(792, 488)
        Me._picFace_244.Name = "_picFace_244"
        Me._picFace_244.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_244.Size = New System.Drawing.Size(41, 41)
        Me._picFace_244.TabIndex = 258
        Me._picFace_244.TabStop = False
        '
        '_picFace_243
        '
        Me._picFace_243.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_243.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_243.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_243.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_243.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_243.Image = CType(resources.GetObject("_picFace_243.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_243, CType(243, Short))
        Me._picFace_243.Location = New System.Drawing.Point(832, 488)
        Me._picFace_243.Name = "_picFace_243"
        Me._picFace_243.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_243.Size = New System.Drawing.Size(41, 41)
        Me._picFace_243.TabIndex = 257
        Me._picFace_243.TabStop = False
        '
        '_picFace_242
        '
        Me._picFace_242.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_242.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_242.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_242.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_242.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_242.Image = CType(resources.GetObject("_picFace_242.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_242, CType(242, Short))
        Me._picFace_242.Location = New System.Drawing.Point(872, 488)
        Me._picFace_242.Name = "_picFace_242"
        Me._picFace_242.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_242.Size = New System.Drawing.Size(41, 41)
        Me._picFace_242.TabIndex = 256
        Me._picFace_242.TabStop = False
        '
        '_picFace_241
        '
        Me._picFace_241.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_241.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_241.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_241.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_241.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_241.Image = CType(resources.GetObject("_picFace_241.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_241, CType(241, Short))
        Me._picFace_241.Location = New System.Drawing.Point(912, 488)
        Me._picFace_241.Name = "_picFace_241"
        Me._picFace_241.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_241.Size = New System.Drawing.Size(41, 41)
        Me._picFace_241.TabIndex = 255
        Me._picFace_241.TabStop = False
        '
        '_picFace_240
        '
        Me._picFace_240.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_240.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_240.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_240.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_240.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_240.Image = CType(resources.GetObject("_picFace_240.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_240, CType(240, Short))
        Me._picFace_240.Location = New System.Drawing.Point(952, 488)
        Me._picFace_240.Name = "_picFace_240"
        Me._picFace_240.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_240.Size = New System.Drawing.Size(41, 41)
        Me._picFace_240.TabIndex = 254
        Me._picFace_240.TabStop = False
        '
        '_picFace_239
        '
        Me._picFace_239.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_239.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_239.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_239.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_239.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_239.Image = CType(resources.GetObject("_picFace_239.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_239, CType(239, Short))
        Me._picFace_239.Location = New System.Drawing.Point(192, 448)
        Me._picFace_239.Name = "_picFace_239"
        Me._picFace_239.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_239.Size = New System.Drawing.Size(41, 41)
        Me._picFace_239.TabIndex = 253
        Me._picFace_239.TabStop = False
        '
        '_picFace_238
        '
        Me._picFace_238.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_238.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_238.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_238.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_238.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_238.Image = CType(resources.GetObject("_picFace_238.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_238, CType(238, Short))
        Me._picFace_238.Location = New System.Drawing.Point(232, 448)
        Me._picFace_238.Name = "_picFace_238"
        Me._picFace_238.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_238.Size = New System.Drawing.Size(41, 41)
        Me._picFace_238.TabIndex = 252
        Me._picFace_238.TabStop = False
        '
        '_picFace_237
        '
        Me._picFace_237.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_237.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_237.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_237.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_237.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_237.Image = CType(resources.GetObject("_picFace_237.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_237, CType(237, Short))
        Me._picFace_237.Location = New System.Drawing.Point(272, 448)
        Me._picFace_237.Name = "_picFace_237"
        Me._picFace_237.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_237.Size = New System.Drawing.Size(41, 41)
        Me._picFace_237.TabIndex = 251
        Me._picFace_237.TabStop = False
        '
        '_picFace_236
        '
        Me._picFace_236.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_236.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_236.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_236.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_236.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_236.Image = CType(resources.GetObject("_picFace_236.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_236, CType(236, Short))
        Me._picFace_236.Location = New System.Drawing.Point(312, 448)
        Me._picFace_236.Name = "_picFace_236"
        Me._picFace_236.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_236.Size = New System.Drawing.Size(41, 41)
        Me._picFace_236.TabIndex = 250
        Me._picFace_236.TabStop = False
        '
        '_picFace_235
        '
        Me._picFace_235.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_235.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_235.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_235.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_235.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_235.Image = CType(resources.GetObject("_picFace_235.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_235, CType(235, Short))
        Me._picFace_235.Location = New System.Drawing.Point(352, 448)
        Me._picFace_235.Name = "_picFace_235"
        Me._picFace_235.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_235.Size = New System.Drawing.Size(41, 41)
        Me._picFace_235.TabIndex = 249
        Me._picFace_235.TabStop = False
        '
        '_picFace_234
        '
        Me._picFace_234.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_234.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_234.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_234.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_234.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_234.Image = CType(resources.GetObject("_picFace_234.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_234, CType(234, Short))
        Me._picFace_234.Location = New System.Drawing.Point(392, 448)
        Me._picFace_234.Name = "_picFace_234"
        Me._picFace_234.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_234.Size = New System.Drawing.Size(41, 41)
        Me._picFace_234.TabIndex = 248
        Me._picFace_234.TabStop = False
        '
        '_picFace_233
        '
        Me._picFace_233.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_233.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_233.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_233.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_233.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_233.Image = CType(resources.GetObject("_picFace_233.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_233, CType(233, Short))
        Me._picFace_233.Location = New System.Drawing.Point(432, 448)
        Me._picFace_233.Name = "_picFace_233"
        Me._picFace_233.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_233.Size = New System.Drawing.Size(41, 41)
        Me._picFace_233.TabIndex = 247
        Me._picFace_233.TabStop = False
        '
        '_picFace_232
        '
        Me._picFace_232.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_232.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_232.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_232.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_232.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_232.Image = CType(resources.GetObject("_picFace_232.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_232, CType(232, Short))
        Me._picFace_232.Location = New System.Drawing.Point(472, 448)
        Me._picFace_232.Name = "_picFace_232"
        Me._picFace_232.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_232.Size = New System.Drawing.Size(41, 41)
        Me._picFace_232.TabIndex = 246
        Me._picFace_232.TabStop = False
        '
        '_picFace_231
        '
        Me._picFace_231.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_231.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_231.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_231.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_231.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_231.Image = CType(resources.GetObject("_picFace_231.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_231, CType(231, Short))
        Me._picFace_231.Location = New System.Drawing.Point(512, 448)
        Me._picFace_231.Name = "_picFace_231"
        Me._picFace_231.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_231.Size = New System.Drawing.Size(41, 41)
        Me._picFace_231.TabIndex = 245
        Me._picFace_231.TabStop = False
        '
        '_picFace_230
        '
        Me._picFace_230.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_230.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_230.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_230.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_230.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_230.Image = CType(resources.GetObject("_picFace_230.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_230, CType(230, Short))
        Me._picFace_230.Location = New System.Drawing.Point(552, 448)
        Me._picFace_230.Name = "_picFace_230"
        Me._picFace_230.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_230.Size = New System.Drawing.Size(41, 41)
        Me._picFace_230.TabIndex = 244
        Me._picFace_230.TabStop = False
        '
        '_picFace_229
        '
        Me._picFace_229.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_229.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_229.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_229.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_229.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_229.Image = CType(resources.GetObject("_picFace_229.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_229, CType(229, Short))
        Me._picFace_229.Location = New System.Drawing.Point(592, 448)
        Me._picFace_229.Name = "_picFace_229"
        Me._picFace_229.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_229.Size = New System.Drawing.Size(41, 41)
        Me._picFace_229.TabIndex = 243
        Me._picFace_229.TabStop = False
        '
        '_picFace_228
        '
        Me._picFace_228.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_228.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_228.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_228.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_228.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_228.Image = CType(resources.GetObject("_picFace_228.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_228, CType(228, Short))
        Me._picFace_228.Location = New System.Drawing.Point(632, 448)
        Me._picFace_228.Name = "_picFace_228"
        Me._picFace_228.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_228.Size = New System.Drawing.Size(41, 41)
        Me._picFace_228.TabIndex = 242
        Me._picFace_228.TabStop = False
        '
        '_picFace_227
        '
        Me._picFace_227.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_227.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_227.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_227.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_227.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_227.Image = CType(resources.GetObject("_picFace_227.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_227, CType(227, Short))
        Me._picFace_227.Location = New System.Drawing.Point(672, 448)
        Me._picFace_227.Name = "_picFace_227"
        Me._picFace_227.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_227.Size = New System.Drawing.Size(41, 41)
        Me._picFace_227.TabIndex = 241
        Me._picFace_227.TabStop = False
        '
        '_picFace_226
        '
        Me._picFace_226.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_226.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_226.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_226.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_226.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_226.Image = CType(resources.GetObject("_picFace_226.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_226, CType(226, Short))
        Me._picFace_226.Location = New System.Drawing.Point(712, 448)
        Me._picFace_226.Name = "_picFace_226"
        Me._picFace_226.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_226.Size = New System.Drawing.Size(41, 41)
        Me._picFace_226.TabIndex = 240
        Me._picFace_226.TabStop = False
        '
        '_picFace_225
        '
        Me._picFace_225.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_225.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_225.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_225.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_225.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_225.Image = CType(resources.GetObject("_picFace_225.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_225, CType(225, Short))
        Me._picFace_225.Location = New System.Drawing.Point(752, 448)
        Me._picFace_225.Name = "_picFace_225"
        Me._picFace_225.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_225.Size = New System.Drawing.Size(41, 41)
        Me._picFace_225.TabIndex = 239
        Me._picFace_225.TabStop = False
        '
        '_picFace_224
        '
        Me._picFace_224.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_224.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_224.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_224.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_224.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_224.Image = CType(resources.GetObject("_picFace_224.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_224, CType(224, Short))
        Me._picFace_224.Location = New System.Drawing.Point(792, 448)
        Me._picFace_224.Name = "_picFace_224"
        Me._picFace_224.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_224.Size = New System.Drawing.Size(41, 41)
        Me._picFace_224.TabIndex = 238
        Me._picFace_224.TabStop = False
        '
        '_picFace_223
        '
        Me._picFace_223.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_223.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_223.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_223.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_223.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_223.Image = CType(resources.GetObject("_picFace_223.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_223, CType(223, Short))
        Me._picFace_223.Location = New System.Drawing.Point(832, 448)
        Me._picFace_223.Name = "_picFace_223"
        Me._picFace_223.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_223.Size = New System.Drawing.Size(41, 41)
        Me._picFace_223.TabIndex = 237
        Me._picFace_223.TabStop = False
        '
        '_picFace_222
        '
        Me._picFace_222.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_222.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_222.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_222.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_222.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_222.Image = CType(resources.GetObject("_picFace_222.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_222, CType(222, Short))
        Me._picFace_222.Location = New System.Drawing.Point(872, 448)
        Me._picFace_222.Name = "_picFace_222"
        Me._picFace_222.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_222.Size = New System.Drawing.Size(41, 41)
        Me._picFace_222.TabIndex = 236
        Me._picFace_222.TabStop = False
        '
        '_picFace_221
        '
        Me._picFace_221.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_221.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_221.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_221.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_221.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_221.Image = CType(resources.GetObject("_picFace_221.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_221, CType(221, Short))
        Me._picFace_221.Location = New System.Drawing.Point(912, 448)
        Me._picFace_221.Name = "_picFace_221"
        Me._picFace_221.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_221.Size = New System.Drawing.Size(41, 41)
        Me._picFace_221.TabIndex = 235
        Me._picFace_221.TabStop = False
        '
        '_picFace_220
        '
        Me._picFace_220.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_220.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_220.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_220.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_220.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_220.Image = CType(resources.GetObject("_picFace_220.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_220, CType(220, Short))
        Me._picFace_220.Location = New System.Drawing.Point(952, 448)
        Me._picFace_220.Name = "_picFace_220"
        Me._picFace_220.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_220.Size = New System.Drawing.Size(41, 41)
        Me._picFace_220.TabIndex = 234
        Me._picFace_220.TabStop = False
        '
        '_picFace_219
        '
        Me._picFace_219.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_219.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_219.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_219.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_219.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_219.Image = CType(resources.GetObject("_picFace_219.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_219, CType(219, Short))
        Me._picFace_219.Location = New System.Drawing.Point(192, 408)
        Me._picFace_219.Name = "_picFace_219"
        Me._picFace_219.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_219.Size = New System.Drawing.Size(41, 41)
        Me._picFace_219.TabIndex = 233
        Me._picFace_219.TabStop = False
        '
        '_picFace_218
        '
        Me._picFace_218.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_218.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_218.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_218.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_218.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_218.Image = CType(resources.GetObject("_picFace_218.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_218, CType(218, Short))
        Me._picFace_218.Location = New System.Drawing.Point(232, 408)
        Me._picFace_218.Name = "_picFace_218"
        Me._picFace_218.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_218.Size = New System.Drawing.Size(41, 41)
        Me._picFace_218.TabIndex = 232
        Me._picFace_218.TabStop = False
        '
        '_picFace_217
        '
        Me._picFace_217.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_217.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_217.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_217.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_217.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_217.Image = CType(resources.GetObject("_picFace_217.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_217, CType(217, Short))
        Me._picFace_217.Location = New System.Drawing.Point(272, 408)
        Me._picFace_217.Name = "_picFace_217"
        Me._picFace_217.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_217.Size = New System.Drawing.Size(41, 41)
        Me._picFace_217.TabIndex = 231
        Me._picFace_217.TabStop = False
        '
        '_picFace_216
        '
        Me._picFace_216.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_216.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_216.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_216.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_216.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_216.Image = CType(resources.GetObject("_picFace_216.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_216, CType(216, Short))
        Me._picFace_216.Location = New System.Drawing.Point(312, 408)
        Me._picFace_216.Name = "_picFace_216"
        Me._picFace_216.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_216.Size = New System.Drawing.Size(41, 41)
        Me._picFace_216.TabIndex = 230
        Me._picFace_216.TabStop = False
        '
        '_picFace_215
        '
        Me._picFace_215.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_215.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_215.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_215.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_215.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_215.Image = CType(resources.GetObject("_picFace_215.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_215, CType(215, Short))
        Me._picFace_215.Location = New System.Drawing.Point(352, 408)
        Me._picFace_215.Name = "_picFace_215"
        Me._picFace_215.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_215.Size = New System.Drawing.Size(41, 41)
        Me._picFace_215.TabIndex = 229
        Me._picFace_215.TabStop = False
        '
        '_picFace_214
        '
        Me._picFace_214.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_214.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_214.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_214.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_214.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_214.Image = CType(resources.GetObject("_picFace_214.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_214, CType(214, Short))
        Me._picFace_214.Location = New System.Drawing.Point(392, 408)
        Me._picFace_214.Name = "_picFace_214"
        Me._picFace_214.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_214.Size = New System.Drawing.Size(41, 41)
        Me._picFace_214.TabIndex = 228
        Me._picFace_214.TabStop = False
        '
        '_picFace_213
        '
        Me._picFace_213.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_213.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_213.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_213.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_213.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_213.Image = CType(resources.GetObject("_picFace_213.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_213, CType(213, Short))
        Me._picFace_213.Location = New System.Drawing.Point(432, 408)
        Me._picFace_213.Name = "_picFace_213"
        Me._picFace_213.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_213.Size = New System.Drawing.Size(41, 41)
        Me._picFace_213.TabIndex = 227
        Me._picFace_213.TabStop = False
        '
        '_picFace_212
        '
        Me._picFace_212.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_212.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_212.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_212.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_212.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_212.Image = CType(resources.GetObject("_picFace_212.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_212, CType(212, Short))
        Me._picFace_212.Location = New System.Drawing.Point(472, 408)
        Me._picFace_212.Name = "_picFace_212"
        Me._picFace_212.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_212.Size = New System.Drawing.Size(41, 41)
        Me._picFace_212.TabIndex = 226
        Me._picFace_212.TabStop = False
        '
        '_picFace_211
        '
        Me._picFace_211.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_211.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_211.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_211.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_211.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_211.Image = CType(resources.GetObject("_picFace_211.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_211, CType(211, Short))
        Me._picFace_211.Location = New System.Drawing.Point(512, 408)
        Me._picFace_211.Name = "_picFace_211"
        Me._picFace_211.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_211.Size = New System.Drawing.Size(41, 41)
        Me._picFace_211.TabIndex = 225
        Me._picFace_211.TabStop = False
        '
        '_picFace_210
        '
        Me._picFace_210.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_210.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_210.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_210.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_210.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_210.Image = CType(resources.GetObject("_picFace_210.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_210, CType(210, Short))
        Me._picFace_210.Location = New System.Drawing.Point(552, 408)
        Me._picFace_210.Name = "_picFace_210"
        Me._picFace_210.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_210.Size = New System.Drawing.Size(41, 41)
        Me._picFace_210.TabIndex = 224
        Me._picFace_210.TabStop = False
        '
        '_picFace_209
        '
        Me._picFace_209.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_209.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_209.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_209.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_209.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_209.Image = CType(resources.GetObject("_picFace_209.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_209, CType(209, Short))
        Me._picFace_209.Location = New System.Drawing.Point(592, 408)
        Me._picFace_209.Name = "_picFace_209"
        Me._picFace_209.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_209.Size = New System.Drawing.Size(41, 41)
        Me._picFace_209.TabIndex = 223
        Me._picFace_209.TabStop = False
        '
        '_picFace_208
        '
        Me._picFace_208.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_208.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_208.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_208.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_208.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_208.Image = CType(resources.GetObject("_picFace_208.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_208, CType(208, Short))
        Me._picFace_208.Location = New System.Drawing.Point(632, 408)
        Me._picFace_208.Name = "_picFace_208"
        Me._picFace_208.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_208.Size = New System.Drawing.Size(41, 41)
        Me._picFace_208.TabIndex = 222
        Me._picFace_208.TabStop = False
        '
        '_picFace_207
        '
        Me._picFace_207.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_207.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_207.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_207.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_207.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_207.Image = CType(resources.GetObject("_picFace_207.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_207, CType(207, Short))
        Me._picFace_207.Location = New System.Drawing.Point(672, 408)
        Me._picFace_207.Name = "_picFace_207"
        Me._picFace_207.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_207.Size = New System.Drawing.Size(41, 41)
        Me._picFace_207.TabIndex = 221
        Me._picFace_207.TabStop = False
        '
        '_picFace_206
        '
        Me._picFace_206.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_206.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_206.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_206.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_206.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_206.Image = CType(resources.GetObject("_picFace_206.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_206, CType(206, Short))
        Me._picFace_206.Location = New System.Drawing.Point(712, 408)
        Me._picFace_206.Name = "_picFace_206"
        Me._picFace_206.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_206.Size = New System.Drawing.Size(41, 41)
        Me._picFace_206.TabIndex = 220
        Me._picFace_206.TabStop = False
        '
        '_picFace_205
        '
        Me._picFace_205.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_205.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_205.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_205.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_205.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_205.Image = CType(resources.GetObject("_picFace_205.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_205, CType(205, Short))
        Me._picFace_205.Location = New System.Drawing.Point(752, 408)
        Me._picFace_205.Name = "_picFace_205"
        Me._picFace_205.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_205.Size = New System.Drawing.Size(41, 41)
        Me._picFace_205.TabIndex = 219
        Me._picFace_205.TabStop = False
        '
        '_picFace_204
        '
        Me._picFace_204.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_204.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_204.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_204.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_204.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_204.Image = CType(resources.GetObject("_picFace_204.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_204, CType(204, Short))
        Me._picFace_204.Location = New System.Drawing.Point(792, 408)
        Me._picFace_204.Name = "_picFace_204"
        Me._picFace_204.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_204.Size = New System.Drawing.Size(41, 41)
        Me._picFace_204.TabIndex = 218
        Me._picFace_204.TabStop = False
        '
        '_picFace_203
        '
        Me._picFace_203.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_203.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_203.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_203.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_203.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_203.Image = CType(resources.GetObject("_picFace_203.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_203, CType(203, Short))
        Me._picFace_203.Location = New System.Drawing.Point(832, 408)
        Me._picFace_203.Name = "_picFace_203"
        Me._picFace_203.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_203.Size = New System.Drawing.Size(41, 41)
        Me._picFace_203.TabIndex = 217
        Me._picFace_203.TabStop = False
        '
        '_picFace_202
        '
        Me._picFace_202.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_202.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_202.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_202.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_202.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_202.Image = CType(resources.GetObject("_picFace_202.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_202, CType(202, Short))
        Me._picFace_202.Location = New System.Drawing.Point(872, 408)
        Me._picFace_202.Name = "_picFace_202"
        Me._picFace_202.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_202.Size = New System.Drawing.Size(41, 41)
        Me._picFace_202.TabIndex = 216
        Me._picFace_202.TabStop = False
        '
        '_picFace_201
        '
        Me._picFace_201.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_201.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_201.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_201.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_201.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_201.Image = CType(resources.GetObject("_picFace_201.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_201, CType(201, Short))
        Me._picFace_201.Location = New System.Drawing.Point(912, 408)
        Me._picFace_201.Name = "_picFace_201"
        Me._picFace_201.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_201.Size = New System.Drawing.Size(41, 41)
        Me._picFace_201.TabIndex = 215
        Me._picFace_201.TabStop = False
        '
        '_picFace_200
        '
        Me._picFace_200.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_200.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_200.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_200.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_200.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_200.Image = CType(resources.GetObject("_picFace_200.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_200, CType(200, Short))
        Me._picFace_200.Location = New System.Drawing.Point(952, 408)
        Me._picFace_200.Name = "_picFace_200"
        Me._picFace_200.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_200.Size = New System.Drawing.Size(41, 41)
        Me._picFace_200.TabIndex = 214
        Me._picFace_200.TabStop = False
        '
        '_picFace_199
        '
        Me._picFace_199.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_199.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_199.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_199.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_199.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_199.Image = CType(resources.GetObject("_picFace_199.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_199, CType(199, Short))
        Me._picFace_199.Location = New System.Drawing.Point(192, 368)
        Me._picFace_199.Name = "_picFace_199"
        Me._picFace_199.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_199.Size = New System.Drawing.Size(41, 41)
        Me._picFace_199.TabIndex = 213
        Me._picFace_199.TabStop = False
        '
        '_picFace_198
        '
        Me._picFace_198.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_198.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_198.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_198.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_198.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_198.Image = CType(resources.GetObject("_picFace_198.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_198, CType(198, Short))
        Me._picFace_198.Location = New System.Drawing.Point(232, 368)
        Me._picFace_198.Name = "_picFace_198"
        Me._picFace_198.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_198.Size = New System.Drawing.Size(41, 41)
        Me._picFace_198.TabIndex = 212
        Me._picFace_198.TabStop = False
        '
        '_picFace_197
        '
        Me._picFace_197.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_197.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_197.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_197.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_197.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_197.Image = CType(resources.GetObject("_picFace_197.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_197, CType(197, Short))
        Me._picFace_197.Location = New System.Drawing.Point(272, 368)
        Me._picFace_197.Name = "_picFace_197"
        Me._picFace_197.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_197.Size = New System.Drawing.Size(41, 41)
        Me._picFace_197.TabIndex = 211
        Me._picFace_197.TabStop = False
        '
        '_picFace_196
        '
        Me._picFace_196.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_196.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_196.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_196.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_196.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_196.Image = CType(resources.GetObject("_picFace_196.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_196, CType(196, Short))
        Me._picFace_196.Location = New System.Drawing.Point(312, 368)
        Me._picFace_196.Name = "_picFace_196"
        Me._picFace_196.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_196.Size = New System.Drawing.Size(41, 41)
        Me._picFace_196.TabIndex = 210
        Me._picFace_196.TabStop = False
        '
        '_picFace_195
        '
        Me._picFace_195.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_195.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_195.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_195.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_195.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_195.Image = CType(resources.GetObject("_picFace_195.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_195, CType(195, Short))
        Me._picFace_195.Location = New System.Drawing.Point(352, 368)
        Me._picFace_195.Name = "_picFace_195"
        Me._picFace_195.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_195.Size = New System.Drawing.Size(41, 41)
        Me._picFace_195.TabIndex = 209
        Me._picFace_195.TabStop = False
        '
        '_picFace_194
        '
        Me._picFace_194.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_194.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_194.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_194.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_194.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_194.Image = CType(resources.GetObject("_picFace_194.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_194, CType(194, Short))
        Me._picFace_194.Location = New System.Drawing.Point(392, 368)
        Me._picFace_194.Name = "_picFace_194"
        Me._picFace_194.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_194.Size = New System.Drawing.Size(41, 41)
        Me._picFace_194.TabIndex = 208
        Me._picFace_194.TabStop = False
        '
        '_picFace_193
        '
        Me._picFace_193.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_193.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_193.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_193.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_193.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_193.Image = CType(resources.GetObject("_picFace_193.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_193, CType(193, Short))
        Me._picFace_193.Location = New System.Drawing.Point(432, 368)
        Me._picFace_193.Name = "_picFace_193"
        Me._picFace_193.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_193.Size = New System.Drawing.Size(41, 41)
        Me._picFace_193.TabIndex = 207
        Me._picFace_193.TabStop = False
        '
        '_picFace_192
        '
        Me._picFace_192.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_192.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_192.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_192.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_192.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_192.Image = CType(resources.GetObject("_picFace_192.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_192, CType(192, Short))
        Me._picFace_192.Location = New System.Drawing.Point(472, 368)
        Me._picFace_192.Name = "_picFace_192"
        Me._picFace_192.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_192.Size = New System.Drawing.Size(41, 41)
        Me._picFace_192.TabIndex = 206
        Me._picFace_192.TabStop = False
        '
        '_picFace_191
        '
        Me._picFace_191.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_191.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_191.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_191.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_191.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_191.Image = CType(resources.GetObject("_picFace_191.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_191, CType(191, Short))
        Me._picFace_191.Location = New System.Drawing.Point(512, 368)
        Me._picFace_191.Name = "_picFace_191"
        Me._picFace_191.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_191.Size = New System.Drawing.Size(41, 41)
        Me._picFace_191.TabIndex = 205
        Me._picFace_191.TabStop = False
        '
        '_picFace_190
        '
        Me._picFace_190.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_190.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_190.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_190.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_190.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_190.Image = CType(resources.GetObject("_picFace_190.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_190, CType(190, Short))
        Me._picFace_190.Location = New System.Drawing.Point(552, 368)
        Me._picFace_190.Name = "_picFace_190"
        Me._picFace_190.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_190.Size = New System.Drawing.Size(41, 41)
        Me._picFace_190.TabIndex = 204
        Me._picFace_190.TabStop = False
        '
        '_picFace_189
        '
        Me._picFace_189.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_189.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_189.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_189.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_189.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_189.Image = CType(resources.GetObject("_picFace_189.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_189, CType(189, Short))
        Me._picFace_189.Location = New System.Drawing.Point(592, 368)
        Me._picFace_189.Name = "_picFace_189"
        Me._picFace_189.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_189.Size = New System.Drawing.Size(41, 41)
        Me._picFace_189.TabIndex = 203
        Me._picFace_189.TabStop = False
        '
        '_picFace_188
        '
        Me._picFace_188.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_188.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_188.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_188.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_188.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_188.Image = CType(resources.GetObject("_picFace_188.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_188, CType(188, Short))
        Me._picFace_188.Location = New System.Drawing.Point(632, 368)
        Me._picFace_188.Name = "_picFace_188"
        Me._picFace_188.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_188.Size = New System.Drawing.Size(41, 41)
        Me._picFace_188.TabIndex = 202
        Me._picFace_188.TabStop = False
        '
        '_picFace_187
        '
        Me._picFace_187.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_187.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_187.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_187.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_187.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_187.Image = CType(resources.GetObject("_picFace_187.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_187, CType(187, Short))
        Me._picFace_187.Location = New System.Drawing.Point(672, 368)
        Me._picFace_187.Name = "_picFace_187"
        Me._picFace_187.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_187.Size = New System.Drawing.Size(41, 41)
        Me._picFace_187.TabIndex = 201
        Me._picFace_187.TabStop = False
        '
        '_picFace_186
        '
        Me._picFace_186.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_186.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_186.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_186.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_186.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_186.Image = CType(resources.GetObject("_picFace_186.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_186, CType(186, Short))
        Me._picFace_186.Location = New System.Drawing.Point(712, 368)
        Me._picFace_186.Name = "_picFace_186"
        Me._picFace_186.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_186.Size = New System.Drawing.Size(41, 41)
        Me._picFace_186.TabIndex = 200
        Me._picFace_186.TabStop = False
        '
        '_picFace_185
        '
        Me._picFace_185.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_185.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_185.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_185.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_185.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_185.Image = CType(resources.GetObject("_picFace_185.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_185, CType(185, Short))
        Me._picFace_185.Location = New System.Drawing.Point(752, 368)
        Me._picFace_185.Name = "_picFace_185"
        Me._picFace_185.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_185.Size = New System.Drawing.Size(41, 41)
        Me._picFace_185.TabIndex = 199
        Me._picFace_185.TabStop = False
        '
        '_picFace_184
        '
        Me._picFace_184.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_184.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_184.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_184.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_184.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_184.Image = CType(resources.GetObject("_picFace_184.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_184, CType(184, Short))
        Me._picFace_184.Location = New System.Drawing.Point(792, 368)
        Me._picFace_184.Name = "_picFace_184"
        Me._picFace_184.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_184.Size = New System.Drawing.Size(41, 41)
        Me._picFace_184.TabIndex = 198
        Me._picFace_184.TabStop = False
        '
        '_picFace_183
        '
        Me._picFace_183.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_183.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_183.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_183.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_183.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_183.Image = CType(resources.GetObject("_picFace_183.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_183, CType(183, Short))
        Me._picFace_183.Location = New System.Drawing.Point(832, 368)
        Me._picFace_183.Name = "_picFace_183"
        Me._picFace_183.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_183.Size = New System.Drawing.Size(41, 41)
        Me._picFace_183.TabIndex = 197
        Me._picFace_183.TabStop = False
        '
        '_picFace_182
        '
        Me._picFace_182.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_182.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_182.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_182.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_182.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_182.Image = CType(resources.GetObject("_picFace_182.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_182, CType(182, Short))
        Me._picFace_182.Location = New System.Drawing.Point(872, 368)
        Me._picFace_182.Name = "_picFace_182"
        Me._picFace_182.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_182.Size = New System.Drawing.Size(41, 41)
        Me._picFace_182.TabIndex = 196
        Me._picFace_182.TabStop = False
        '
        '_picFace_181
        '
        Me._picFace_181.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_181.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_181.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_181.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_181.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_181.Image = CType(resources.GetObject("_picFace_181.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_181, CType(181, Short))
        Me._picFace_181.Location = New System.Drawing.Point(912, 368)
        Me._picFace_181.Name = "_picFace_181"
        Me._picFace_181.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_181.Size = New System.Drawing.Size(41, 41)
        Me._picFace_181.TabIndex = 195
        Me._picFace_181.TabStop = False
        '
        '_picFace_180
        '
        Me._picFace_180.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_180.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_180.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_180.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_180.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_180.Image = CType(resources.GetObject("_picFace_180.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_180, CType(180, Short))
        Me._picFace_180.Location = New System.Drawing.Point(952, 368)
        Me._picFace_180.Name = "_picFace_180"
        Me._picFace_180.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_180.Size = New System.Drawing.Size(41, 41)
        Me._picFace_180.TabIndex = 194
        Me._picFace_180.TabStop = False
        '
        '_picFace_179
        '
        Me._picFace_179.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_179.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_179.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_179.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_179.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_179.Image = CType(resources.GetObject("_picFace_179.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_179, CType(179, Short))
        Me._picFace_179.Location = New System.Drawing.Point(192, 328)
        Me._picFace_179.Name = "_picFace_179"
        Me._picFace_179.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_179.Size = New System.Drawing.Size(41, 41)
        Me._picFace_179.TabIndex = 193
        Me._picFace_179.TabStop = False
        '
        '_picFace_178
        '
        Me._picFace_178.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_178.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_178.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_178.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_178.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_178.Image = CType(resources.GetObject("_picFace_178.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_178, CType(178, Short))
        Me._picFace_178.Location = New System.Drawing.Point(232, 328)
        Me._picFace_178.Name = "_picFace_178"
        Me._picFace_178.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_178.Size = New System.Drawing.Size(41, 41)
        Me._picFace_178.TabIndex = 192
        Me._picFace_178.TabStop = False
        '
        '_picFace_177
        '
        Me._picFace_177.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_177.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_177.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_177.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_177.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_177.Image = CType(resources.GetObject("_picFace_177.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_177, CType(177, Short))
        Me._picFace_177.Location = New System.Drawing.Point(272, 328)
        Me._picFace_177.Name = "_picFace_177"
        Me._picFace_177.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_177.Size = New System.Drawing.Size(41, 41)
        Me._picFace_177.TabIndex = 191
        Me._picFace_177.TabStop = False
        '
        '_picFace_176
        '
        Me._picFace_176.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_176.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_176.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_176.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_176.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_176.Image = CType(resources.GetObject("_picFace_176.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_176, CType(176, Short))
        Me._picFace_176.Location = New System.Drawing.Point(312, 328)
        Me._picFace_176.Name = "_picFace_176"
        Me._picFace_176.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_176.Size = New System.Drawing.Size(41, 41)
        Me._picFace_176.TabIndex = 190
        Me._picFace_176.TabStop = False
        '
        '_picFace_175
        '
        Me._picFace_175.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_175.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_175.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_175.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_175.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_175.Image = CType(resources.GetObject("_picFace_175.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_175, CType(175, Short))
        Me._picFace_175.Location = New System.Drawing.Point(352, 328)
        Me._picFace_175.Name = "_picFace_175"
        Me._picFace_175.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_175.Size = New System.Drawing.Size(41, 41)
        Me._picFace_175.TabIndex = 189
        Me._picFace_175.TabStop = False
        '
        '_picFace_174
        '
        Me._picFace_174.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_174.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_174.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_174.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_174.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_174.Image = CType(resources.GetObject("_picFace_174.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_174, CType(174, Short))
        Me._picFace_174.Location = New System.Drawing.Point(392, 328)
        Me._picFace_174.Name = "_picFace_174"
        Me._picFace_174.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_174.Size = New System.Drawing.Size(41, 41)
        Me._picFace_174.TabIndex = 188
        Me._picFace_174.TabStop = False
        '
        '_picFace_173
        '
        Me._picFace_173.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_173.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_173.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_173.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_173.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_173.Image = CType(resources.GetObject("_picFace_173.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_173, CType(173, Short))
        Me._picFace_173.Location = New System.Drawing.Point(432, 328)
        Me._picFace_173.Name = "_picFace_173"
        Me._picFace_173.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_173.Size = New System.Drawing.Size(41, 41)
        Me._picFace_173.TabIndex = 187
        Me._picFace_173.TabStop = False
        '
        '_picFace_172
        '
        Me._picFace_172.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_172.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_172.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_172.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_172.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_172.Image = CType(resources.GetObject("_picFace_172.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_172, CType(172, Short))
        Me._picFace_172.Location = New System.Drawing.Point(472, 328)
        Me._picFace_172.Name = "_picFace_172"
        Me._picFace_172.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_172.Size = New System.Drawing.Size(41, 41)
        Me._picFace_172.TabIndex = 186
        Me._picFace_172.TabStop = False
        '
        '_picFace_171
        '
        Me._picFace_171.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_171.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_171.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_171.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_171.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_171.Image = CType(resources.GetObject("_picFace_171.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_171, CType(171, Short))
        Me._picFace_171.Location = New System.Drawing.Point(512, 328)
        Me._picFace_171.Name = "_picFace_171"
        Me._picFace_171.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_171.Size = New System.Drawing.Size(41, 41)
        Me._picFace_171.TabIndex = 185
        Me._picFace_171.TabStop = False
        '
        '_picFace_170
        '
        Me._picFace_170.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_170.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_170.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_170.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_170.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_170.Image = CType(resources.GetObject("_picFace_170.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_170, CType(170, Short))
        Me._picFace_170.Location = New System.Drawing.Point(552, 328)
        Me._picFace_170.Name = "_picFace_170"
        Me._picFace_170.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_170.Size = New System.Drawing.Size(41, 41)
        Me._picFace_170.TabIndex = 184
        Me._picFace_170.TabStop = False
        '
        '_picFace_169
        '
        Me._picFace_169.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_169.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_169.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_169.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_169.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_169.Image = CType(resources.GetObject("_picFace_169.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_169, CType(169, Short))
        Me._picFace_169.Location = New System.Drawing.Point(592, 328)
        Me._picFace_169.Name = "_picFace_169"
        Me._picFace_169.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_169.Size = New System.Drawing.Size(41, 41)
        Me._picFace_169.TabIndex = 183
        Me._picFace_169.TabStop = False
        '
        '_picFace_168
        '
        Me._picFace_168.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_168.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_168.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_168.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_168.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_168.Image = CType(resources.GetObject("_picFace_168.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_168, CType(168, Short))
        Me._picFace_168.Location = New System.Drawing.Point(632, 328)
        Me._picFace_168.Name = "_picFace_168"
        Me._picFace_168.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_168.Size = New System.Drawing.Size(41, 41)
        Me._picFace_168.TabIndex = 182
        Me._picFace_168.TabStop = False
        '
        '_picFace_167
        '
        Me._picFace_167.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_167.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_167.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_167.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_167.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_167.Image = CType(resources.GetObject("_picFace_167.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_167, CType(167, Short))
        Me._picFace_167.Location = New System.Drawing.Point(672, 328)
        Me._picFace_167.Name = "_picFace_167"
        Me._picFace_167.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_167.Size = New System.Drawing.Size(41, 41)
        Me._picFace_167.TabIndex = 181
        Me._picFace_167.TabStop = False
        '
        '_picFace_166
        '
        Me._picFace_166.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_166.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_166.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_166.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_166.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_166.Image = CType(resources.GetObject("_picFace_166.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_166, CType(166, Short))
        Me._picFace_166.Location = New System.Drawing.Point(712, 328)
        Me._picFace_166.Name = "_picFace_166"
        Me._picFace_166.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_166.Size = New System.Drawing.Size(41, 41)
        Me._picFace_166.TabIndex = 180
        Me._picFace_166.TabStop = False
        '
        '_picFace_165
        '
        Me._picFace_165.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_165.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_165.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_165.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_165.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_165.Image = CType(resources.GetObject("_picFace_165.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_165, CType(165, Short))
        Me._picFace_165.Location = New System.Drawing.Point(752, 328)
        Me._picFace_165.Name = "_picFace_165"
        Me._picFace_165.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_165.Size = New System.Drawing.Size(41, 41)
        Me._picFace_165.TabIndex = 179
        Me._picFace_165.TabStop = False
        '
        '_picFace_164
        '
        Me._picFace_164.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_164.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_164.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_164.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_164.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_164.Image = CType(resources.GetObject("_picFace_164.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_164, CType(164, Short))
        Me._picFace_164.Location = New System.Drawing.Point(792, 328)
        Me._picFace_164.Name = "_picFace_164"
        Me._picFace_164.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_164.Size = New System.Drawing.Size(41, 41)
        Me._picFace_164.TabIndex = 178
        Me._picFace_164.TabStop = False
        '
        '_picFace_163
        '
        Me._picFace_163.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_163.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_163.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_163.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_163.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_163.Image = CType(resources.GetObject("_picFace_163.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_163, CType(163, Short))
        Me._picFace_163.Location = New System.Drawing.Point(832, 328)
        Me._picFace_163.Name = "_picFace_163"
        Me._picFace_163.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_163.Size = New System.Drawing.Size(41, 41)
        Me._picFace_163.TabIndex = 177
        Me._picFace_163.TabStop = False
        '
        '_picFace_162
        '
        Me._picFace_162.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_162.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_162.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_162.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_162.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_162.Image = CType(resources.GetObject("_picFace_162.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_162, CType(162, Short))
        Me._picFace_162.Location = New System.Drawing.Point(872, 328)
        Me._picFace_162.Name = "_picFace_162"
        Me._picFace_162.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_162.Size = New System.Drawing.Size(41, 41)
        Me._picFace_162.TabIndex = 176
        Me._picFace_162.TabStop = False
        '
        '_picFace_161
        '
        Me._picFace_161.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_161.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_161.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_161.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_161.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_161.Image = CType(resources.GetObject("_picFace_161.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_161, CType(161, Short))
        Me._picFace_161.Location = New System.Drawing.Point(912, 328)
        Me._picFace_161.Name = "_picFace_161"
        Me._picFace_161.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_161.Size = New System.Drawing.Size(41, 41)
        Me._picFace_161.TabIndex = 175
        Me._picFace_161.TabStop = False
        '
        '_picFace_160
        '
        Me._picFace_160.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_160.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_160.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_160.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_160.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_160.Image = CType(resources.GetObject("_picFace_160.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_160, CType(160, Short))
        Me._picFace_160.Location = New System.Drawing.Point(952, 328)
        Me._picFace_160.Name = "_picFace_160"
        Me._picFace_160.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_160.Size = New System.Drawing.Size(41, 41)
        Me._picFace_160.TabIndex = 174
        Me._picFace_160.TabStop = False
        '
        '_picFace_159
        '
        Me._picFace_159.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_159.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_159.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_159.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_159.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_159.Image = CType(resources.GetObject("_picFace_159.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_159, CType(159, Short))
        Me._picFace_159.Location = New System.Drawing.Point(192, 288)
        Me._picFace_159.Name = "_picFace_159"
        Me._picFace_159.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_159.Size = New System.Drawing.Size(41, 41)
        Me._picFace_159.TabIndex = 173
        Me._picFace_159.TabStop = False
        '
        '_picFace_158
        '
        Me._picFace_158.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_158.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_158.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_158.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_158.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_158.Image = CType(resources.GetObject("_picFace_158.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_158, CType(158, Short))
        Me._picFace_158.Location = New System.Drawing.Point(232, 288)
        Me._picFace_158.Name = "_picFace_158"
        Me._picFace_158.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_158.Size = New System.Drawing.Size(41, 41)
        Me._picFace_158.TabIndex = 172
        Me._picFace_158.TabStop = False
        '
        '_picFace_157
        '
        Me._picFace_157.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_157.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_157.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_157.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_157.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_157.Image = CType(resources.GetObject("_picFace_157.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_157, CType(157, Short))
        Me._picFace_157.Location = New System.Drawing.Point(272, 288)
        Me._picFace_157.Name = "_picFace_157"
        Me._picFace_157.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_157.Size = New System.Drawing.Size(41, 41)
        Me._picFace_157.TabIndex = 171
        Me._picFace_157.TabStop = False
        '
        '_picFace_156
        '
        Me._picFace_156.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_156.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_156.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_156.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_156.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_156.Image = CType(resources.GetObject("_picFace_156.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_156, CType(156, Short))
        Me._picFace_156.Location = New System.Drawing.Point(312, 288)
        Me._picFace_156.Name = "_picFace_156"
        Me._picFace_156.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_156.Size = New System.Drawing.Size(41, 41)
        Me._picFace_156.TabIndex = 170
        Me._picFace_156.TabStop = False
        '
        '_picFace_155
        '
        Me._picFace_155.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_155.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_155.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_155.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_155.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_155.Image = CType(resources.GetObject("_picFace_155.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_155, CType(155, Short))
        Me._picFace_155.Location = New System.Drawing.Point(352, 288)
        Me._picFace_155.Name = "_picFace_155"
        Me._picFace_155.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_155.Size = New System.Drawing.Size(41, 41)
        Me._picFace_155.TabIndex = 169
        Me._picFace_155.TabStop = False
        '
        '_picFace_154
        '
        Me._picFace_154.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_154.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_154.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_154.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_154.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_154.Image = CType(resources.GetObject("_picFace_154.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_154, CType(154, Short))
        Me._picFace_154.Location = New System.Drawing.Point(392, 288)
        Me._picFace_154.Name = "_picFace_154"
        Me._picFace_154.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_154.Size = New System.Drawing.Size(41, 41)
        Me._picFace_154.TabIndex = 168
        Me._picFace_154.TabStop = False
        '
        '_picFace_153
        '
        Me._picFace_153.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_153.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_153.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_153.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_153.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_153.Image = CType(resources.GetObject("_picFace_153.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_153, CType(153, Short))
        Me._picFace_153.Location = New System.Drawing.Point(432, 288)
        Me._picFace_153.Name = "_picFace_153"
        Me._picFace_153.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_153.Size = New System.Drawing.Size(41, 41)
        Me._picFace_153.TabIndex = 167
        Me._picFace_153.TabStop = False
        '
        '_picFace_152
        '
        Me._picFace_152.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_152.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_152.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_152.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_152.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_152.Image = CType(resources.GetObject("_picFace_152.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_152, CType(152, Short))
        Me._picFace_152.Location = New System.Drawing.Point(472, 288)
        Me._picFace_152.Name = "_picFace_152"
        Me._picFace_152.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_152.Size = New System.Drawing.Size(41, 41)
        Me._picFace_152.TabIndex = 166
        Me._picFace_152.TabStop = False
        '
        '_picFace_151
        '
        Me._picFace_151.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_151.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_151.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_151.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_151.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_151.Image = CType(resources.GetObject("_picFace_151.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_151, CType(151, Short))
        Me._picFace_151.Location = New System.Drawing.Point(512, 288)
        Me._picFace_151.Name = "_picFace_151"
        Me._picFace_151.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_151.Size = New System.Drawing.Size(41, 41)
        Me._picFace_151.TabIndex = 165
        Me._picFace_151.TabStop = False
        '
        '_picFace_150
        '
        Me._picFace_150.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_150.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_150.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_150.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_150.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_150.Image = CType(resources.GetObject("_picFace_150.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_150, CType(150, Short))
        Me._picFace_150.Location = New System.Drawing.Point(552, 288)
        Me._picFace_150.Name = "_picFace_150"
        Me._picFace_150.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_150.Size = New System.Drawing.Size(41, 41)
        Me._picFace_150.TabIndex = 164
        Me._picFace_150.TabStop = False
        '
        '_picFace_149
        '
        Me._picFace_149.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_149.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_149.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_149.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_149.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_149.Image = CType(resources.GetObject("_picFace_149.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_149, CType(149, Short))
        Me._picFace_149.Location = New System.Drawing.Point(592, 288)
        Me._picFace_149.Name = "_picFace_149"
        Me._picFace_149.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_149.Size = New System.Drawing.Size(41, 41)
        Me._picFace_149.TabIndex = 163
        Me._picFace_149.TabStop = False
        '
        '_picFace_148
        '
        Me._picFace_148.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_148.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_148.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_148.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_148.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_148.Image = CType(resources.GetObject("_picFace_148.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_148, CType(148, Short))
        Me._picFace_148.Location = New System.Drawing.Point(632, 288)
        Me._picFace_148.Name = "_picFace_148"
        Me._picFace_148.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_148.Size = New System.Drawing.Size(41, 41)
        Me._picFace_148.TabIndex = 162
        Me._picFace_148.TabStop = False
        '
        '_picFace_147
        '
        Me._picFace_147.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_147.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_147.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_147.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_147.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_147.Image = CType(resources.GetObject("_picFace_147.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_147, CType(147, Short))
        Me._picFace_147.Location = New System.Drawing.Point(672, 288)
        Me._picFace_147.Name = "_picFace_147"
        Me._picFace_147.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_147.Size = New System.Drawing.Size(41, 41)
        Me._picFace_147.TabIndex = 161
        Me._picFace_147.TabStop = False
        '
        '_picFace_146
        '
        Me._picFace_146.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_146.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_146.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_146.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_146.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_146.Image = CType(resources.GetObject("_picFace_146.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_146, CType(146, Short))
        Me._picFace_146.Location = New System.Drawing.Point(712, 288)
        Me._picFace_146.Name = "_picFace_146"
        Me._picFace_146.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_146.Size = New System.Drawing.Size(41, 41)
        Me._picFace_146.TabIndex = 160
        Me._picFace_146.TabStop = False
        '
        '_picFace_145
        '
        Me._picFace_145.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_145.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_145.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_145.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_145.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_145.Image = CType(resources.GetObject("_picFace_145.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_145, CType(145, Short))
        Me._picFace_145.Location = New System.Drawing.Point(752, 288)
        Me._picFace_145.Name = "_picFace_145"
        Me._picFace_145.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_145.Size = New System.Drawing.Size(41, 41)
        Me._picFace_145.TabIndex = 159
        Me._picFace_145.TabStop = False
        '
        '_picFace_144
        '
        Me._picFace_144.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_144.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_144.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_144.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_144.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_144.Image = CType(resources.GetObject("_picFace_144.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_144, CType(144, Short))
        Me._picFace_144.Location = New System.Drawing.Point(792, 288)
        Me._picFace_144.Name = "_picFace_144"
        Me._picFace_144.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_144.Size = New System.Drawing.Size(41, 41)
        Me._picFace_144.TabIndex = 158
        Me._picFace_144.TabStop = False
        '
        '_picFace_143
        '
        Me._picFace_143.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_143.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_143.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_143.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_143.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_143.Image = CType(resources.GetObject("_picFace_143.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_143, CType(143, Short))
        Me._picFace_143.Location = New System.Drawing.Point(832, 288)
        Me._picFace_143.Name = "_picFace_143"
        Me._picFace_143.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_143.Size = New System.Drawing.Size(41, 41)
        Me._picFace_143.TabIndex = 157
        Me._picFace_143.TabStop = False
        '
        '_picFace_142
        '
        Me._picFace_142.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_142.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_142.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_142.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_142.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_142.Image = CType(resources.GetObject("_picFace_142.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_142, CType(142, Short))
        Me._picFace_142.Location = New System.Drawing.Point(872, 288)
        Me._picFace_142.Name = "_picFace_142"
        Me._picFace_142.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_142.Size = New System.Drawing.Size(41, 41)
        Me._picFace_142.TabIndex = 156
        Me._picFace_142.TabStop = False
        '
        '_picFace_141
        '
        Me._picFace_141.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_141.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_141.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_141.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_141.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_141.Image = CType(resources.GetObject("_picFace_141.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_141, CType(141, Short))
        Me._picFace_141.Location = New System.Drawing.Point(912, 288)
        Me._picFace_141.Name = "_picFace_141"
        Me._picFace_141.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_141.Size = New System.Drawing.Size(41, 41)
        Me._picFace_141.TabIndex = 155
        Me._picFace_141.TabStop = False
        '
        '_picFace_140
        '
        Me._picFace_140.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_140.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_140.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_140.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_140.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_140.Image = CType(resources.GetObject("_picFace_140.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_140, CType(140, Short))
        Me._picFace_140.Location = New System.Drawing.Point(952, 288)
        Me._picFace_140.Name = "_picFace_140"
        Me._picFace_140.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_140.Size = New System.Drawing.Size(41, 41)
        Me._picFace_140.TabIndex = 154
        Me._picFace_140.TabStop = False
        '
        '_picFace_139
        '
        Me._picFace_139.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_139.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_139.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_139.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_139.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_139.Image = CType(resources.GetObject("_picFace_139.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_139, CType(139, Short))
        Me._picFace_139.Location = New System.Drawing.Point(192, 248)
        Me._picFace_139.Name = "_picFace_139"
        Me._picFace_139.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_139.Size = New System.Drawing.Size(41, 41)
        Me._picFace_139.TabIndex = 153
        Me._picFace_139.TabStop = False
        '
        '_picFace_138
        '
        Me._picFace_138.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_138.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_138.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_138.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_138.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_138.Image = CType(resources.GetObject("_picFace_138.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_138, CType(138, Short))
        Me._picFace_138.Location = New System.Drawing.Point(232, 248)
        Me._picFace_138.Name = "_picFace_138"
        Me._picFace_138.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_138.Size = New System.Drawing.Size(41, 41)
        Me._picFace_138.TabIndex = 152
        Me._picFace_138.TabStop = False
        '
        '_picFace_137
        '
        Me._picFace_137.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_137.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_137.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_137.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_137.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_137.Image = CType(resources.GetObject("_picFace_137.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_137, CType(137, Short))
        Me._picFace_137.Location = New System.Drawing.Point(272, 248)
        Me._picFace_137.Name = "_picFace_137"
        Me._picFace_137.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_137.Size = New System.Drawing.Size(41, 41)
        Me._picFace_137.TabIndex = 151
        Me._picFace_137.TabStop = False
        '
        '_picFace_136
        '
        Me._picFace_136.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_136.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_136.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_136.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_136.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_136.Image = CType(resources.GetObject("_picFace_136.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_136, CType(136, Short))
        Me._picFace_136.Location = New System.Drawing.Point(312, 248)
        Me._picFace_136.Name = "_picFace_136"
        Me._picFace_136.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_136.Size = New System.Drawing.Size(41, 41)
        Me._picFace_136.TabIndex = 150
        Me._picFace_136.TabStop = False
        '
        '_picFace_135
        '
        Me._picFace_135.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_135.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_135.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_135.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_135.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_135.Image = CType(resources.GetObject("_picFace_135.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_135, CType(135, Short))
        Me._picFace_135.Location = New System.Drawing.Point(352, 248)
        Me._picFace_135.Name = "_picFace_135"
        Me._picFace_135.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_135.Size = New System.Drawing.Size(41, 41)
        Me._picFace_135.TabIndex = 149
        Me._picFace_135.TabStop = False
        '
        '_picFace_134
        '
        Me._picFace_134.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_134.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_134.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_134.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_134.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_134.Image = CType(resources.GetObject("_picFace_134.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_134, CType(134, Short))
        Me._picFace_134.Location = New System.Drawing.Point(392, 248)
        Me._picFace_134.Name = "_picFace_134"
        Me._picFace_134.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_134.Size = New System.Drawing.Size(41, 41)
        Me._picFace_134.TabIndex = 148
        Me._picFace_134.TabStop = False
        '
        '_picFace_133
        '
        Me._picFace_133.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_133.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_133.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_133.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_133.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_133.Image = CType(resources.GetObject("_picFace_133.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_133, CType(133, Short))
        Me._picFace_133.Location = New System.Drawing.Point(432, 248)
        Me._picFace_133.Name = "_picFace_133"
        Me._picFace_133.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_133.Size = New System.Drawing.Size(41, 41)
        Me._picFace_133.TabIndex = 147
        Me._picFace_133.TabStop = False
        '
        '_picFace_132
        '
        Me._picFace_132.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_132.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_132.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_132.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_132.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_132.Image = CType(resources.GetObject("_picFace_132.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_132, CType(132, Short))
        Me._picFace_132.Location = New System.Drawing.Point(472, 248)
        Me._picFace_132.Name = "_picFace_132"
        Me._picFace_132.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_132.Size = New System.Drawing.Size(41, 41)
        Me._picFace_132.TabIndex = 146
        Me._picFace_132.TabStop = False
        '
        '_picFace_131
        '
        Me._picFace_131.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_131.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_131.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_131.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_131.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_131.Image = CType(resources.GetObject("_picFace_131.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_131, CType(131, Short))
        Me._picFace_131.Location = New System.Drawing.Point(512, 248)
        Me._picFace_131.Name = "_picFace_131"
        Me._picFace_131.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_131.Size = New System.Drawing.Size(41, 41)
        Me._picFace_131.TabIndex = 145
        Me._picFace_131.TabStop = False
        '
        '_picFace_130
        '
        Me._picFace_130.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_130.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_130.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_130.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_130.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_130.Image = CType(resources.GetObject("_picFace_130.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_130, CType(130, Short))
        Me._picFace_130.Location = New System.Drawing.Point(552, 248)
        Me._picFace_130.Name = "_picFace_130"
        Me._picFace_130.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_130.Size = New System.Drawing.Size(41, 41)
        Me._picFace_130.TabIndex = 144
        Me._picFace_130.TabStop = False
        '
        '_picFace_129
        '
        Me._picFace_129.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_129.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_129.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_129.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_129.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_129.Image = CType(resources.GetObject("_picFace_129.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_129, CType(129, Short))
        Me._picFace_129.Location = New System.Drawing.Point(592, 248)
        Me._picFace_129.Name = "_picFace_129"
        Me._picFace_129.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_129.Size = New System.Drawing.Size(41, 41)
        Me._picFace_129.TabIndex = 143
        Me._picFace_129.TabStop = False
        '
        '_picFace_128
        '
        Me._picFace_128.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_128.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_128.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_128.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_128.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_128.Image = CType(resources.GetObject("_picFace_128.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_128, CType(128, Short))
        Me._picFace_128.Location = New System.Drawing.Point(632, 248)
        Me._picFace_128.Name = "_picFace_128"
        Me._picFace_128.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_128.Size = New System.Drawing.Size(41, 41)
        Me._picFace_128.TabIndex = 142
        Me._picFace_128.TabStop = False
        '
        '_picFace_127
        '
        Me._picFace_127.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_127.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_127.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_127.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_127.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_127.Image = CType(resources.GetObject("_picFace_127.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_127, CType(127, Short))
        Me._picFace_127.Location = New System.Drawing.Point(672, 248)
        Me._picFace_127.Name = "_picFace_127"
        Me._picFace_127.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_127.Size = New System.Drawing.Size(41, 41)
        Me._picFace_127.TabIndex = 141
        Me._picFace_127.TabStop = False
        '
        '_picFace_126
        '
        Me._picFace_126.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_126.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_126.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_126.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_126.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_126.Image = CType(resources.GetObject("_picFace_126.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_126, CType(126, Short))
        Me._picFace_126.Location = New System.Drawing.Point(712, 248)
        Me._picFace_126.Name = "_picFace_126"
        Me._picFace_126.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_126.Size = New System.Drawing.Size(41, 41)
        Me._picFace_126.TabIndex = 140
        Me._picFace_126.TabStop = False
        '
        '_picFace_125
        '
        Me._picFace_125.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_125.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_125.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_125.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_125.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_125.Image = CType(resources.GetObject("_picFace_125.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_125, CType(125, Short))
        Me._picFace_125.Location = New System.Drawing.Point(752, 248)
        Me._picFace_125.Name = "_picFace_125"
        Me._picFace_125.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_125.Size = New System.Drawing.Size(41, 41)
        Me._picFace_125.TabIndex = 139
        Me._picFace_125.TabStop = False
        '
        '_picFace_124
        '
        Me._picFace_124.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_124.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_124.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_124.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_124.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_124.Image = CType(resources.GetObject("_picFace_124.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_124, CType(124, Short))
        Me._picFace_124.Location = New System.Drawing.Point(792, 248)
        Me._picFace_124.Name = "_picFace_124"
        Me._picFace_124.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_124.Size = New System.Drawing.Size(41, 41)
        Me._picFace_124.TabIndex = 138
        Me._picFace_124.TabStop = False
        '
        '_picFace_123
        '
        Me._picFace_123.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_123.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_123.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_123.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_123.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_123.Image = CType(resources.GetObject("_picFace_123.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_123, CType(123, Short))
        Me._picFace_123.Location = New System.Drawing.Point(832, 248)
        Me._picFace_123.Name = "_picFace_123"
        Me._picFace_123.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_123.Size = New System.Drawing.Size(41, 41)
        Me._picFace_123.TabIndex = 137
        Me._picFace_123.TabStop = False
        '
        '_picFace_122
        '
        Me._picFace_122.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_122.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_122.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_122.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_122.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_122.Image = CType(resources.GetObject("_picFace_122.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_122, CType(122, Short))
        Me._picFace_122.Location = New System.Drawing.Point(872, 248)
        Me._picFace_122.Name = "_picFace_122"
        Me._picFace_122.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_122.Size = New System.Drawing.Size(41, 41)
        Me._picFace_122.TabIndex = 136
        Me._picFace_122.TabStop = False
        '
        '_picFace_121
        '
        Me._picFace_121.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_121.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_121.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_121.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_121.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_121.Image = CType(resources.GetObject("_picFace_121.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_121, CType(121, Short))
        Me._picFace_121.Location = New System.Drawing.Point(912, 248)
        Me._picFace_121.Name = "_picFace_121"
        Me._picFace_121.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_121.Size = New System.Drawing.Size(41, 41)
        Me._picFace_121.TabIndex = 135
        Me._picFace_121.TabStop = False
        '
        '_picFace_120
        '
        Me._picFace_120.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_120.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_120.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_120.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_120.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_120.Image = CType(resources.GetObject("_picFace_120.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_120, CType(120, Short))
        Me._picFace_120.Location = New System.Drawing.Point(952, 248)
        Me._picFace_120.Name = "_picFace_120"
        Me._picFace_120.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_120.Size = New System.Drawing.Size(41, 41)
        Me._picFace_120.TabIndex = 134
        Me._picFace_120.TabStop = False
        '
        '_picFace_119
        '
        Me._picFace_119.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_119.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_119.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_119.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_119.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_119.Image = CType(resources.GetObject("_picFace_119.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_119, CType(119, Short))
        Me._picFace_119.Location = New System.Drawing.Point(192, 208)
        Me._picFace_119.Name = "_picFace_119"
        Me._picFace_119.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_119.Size = New System.Drawing.Size(41, 41)
        Me._picFace_119.TabIndex = 133
        Me._picFace_119.TabStop = False
        '
        '_picFace_118
        '
        Me._picFace_118.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_118.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_118.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_118.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_118.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_118.Image = CType(resources.GetObject("_picFace_118.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_118, CType(118, Short))
        Me._picFace_118.Location = New System.Drawing.Point(232, 208)
        Me._picFace_118.Name = "_picFace_118"
        Me._picFace_118.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_118.Size = New System.Drawing.Size(41, 41)
        Me._picFace_118.TabIndex = 132
        Me._picFace_118.TabStop = False
        '
        '_picFace_117
        '
        Me._picFace_117.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_117.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_117.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_117.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_117.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_117.Image = CType(resources.GetObject("_picFace_117.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_117, CType(117, Short))
        Me._picFace_117.Location = New System.Drawing.Point(272, 208)
        Me._picFace_117.Name = "_picFace_117"
        Me._picFace_117.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_117.Size = New System.Drawing.Size(41, 41)
        Me._picFace_117.TabIndex = 131
        Me._picFace_117.TabStop = False
        '
        '_picFace_116
        '
        Me._picFace_116.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_116.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_116.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_116.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_116.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_116.Image = CType(resources.GetObject("_picFace_116.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_116, CType(116, Short))
        Me._picFace_116.Location = New System.Drawing.Point(312, 208)
        Me._picFace_116.Name = "_picFace_116"
        Me._picFace_116.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_116.Size = New System.Drawing.Size(41, 41)
        Me._picFace_116.TabIndex = 130
        Me._picFace_116.TabStop = False
        '
        '_picFace_115
        '
        Me._picFace_115.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_115.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_115.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_115.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_115.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_115.Image = CType(resources.GetObject("_picFace_115.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_115, CType(115, Short))
        Me._picFace_115.Location = New System.Drawing.Point(352, 208)
        Me._picFace_115.Name = "_picFace_115"
        Me._picFace_115.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_115.Size = New System.Drawing.Size(41, 41)
        Me._picFace_115.TabIndex = 129
        Me._picFace_115.TabStop = False
        '
        '_picFace_114
        '
        Me._picFace_114.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_114.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_114.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_114.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_114.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_114.Image = CType(resources.GetObject("_picFace_114.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_114, CType(114, Short))
        Me._picFace_114.Location = New System.Drawing.Point(392, 208)
        Me._picFace_114.Name = "_picFace_114"
        Me._picFace_114.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_114.Size = New System.Drawing.Size(41, 41)
        Me._picFace_114.TabIndex = 128
        Me._picFace_114.TabStop = False
        '
        '_picFace_113
        '
        Me._picFace_113.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_113.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_113.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_113.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_113.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_113.Image = CType(resources.GetObject("_picFace_113.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_113, CType(113, Short))
        Me._picFace_113.Location = New System.Drawing.Point(432, 208)
        Me._picFace_113.Name = "_picFace_113"
        Me._picFace_113.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_113.Size = New System.Drawing.Size(41, 41)
        Me._picFace_113.TabIndex = 127
        Me._picFace_113.TabStop = False
        '
        '_picFace_112
        '
        Me._picFace_112.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_112.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_112.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_112.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_112.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_112.Image = CType(resources.GetObject("_picFace_112.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_112, CType(112, Short))
        Me._picFace_112.Location = New System.Drawing.Point(472, 208)
        Me._picFace_112.Name = "_picFace_112"
        Me._picFace_112.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_112.Size = New System.Drawing.Size(41, 41)
        Me._picFace_112.TabIndex = 126
        Me._picFace_112.TabStop = False
        '
        '_picFace_111
        '
        Me._picFace_111.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_111.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_111.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_111.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_111.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_111.Image = CType(resources.GetObject("_picFace_111.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_111, CType(111, Short))
        Me._picFace_111.Location = New System.Drawing.Point(512, 208)
        Me._picFace_111.Name = "_picFace_111"
        Me._picFace_111.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_111.Size = New System.Drawing.Size(41, 41)
        Me._picFace_111.TabIndex = 125
        Me._picFace_111.TabStop = False
        '
        '_picFace_110
        '
        Me._picFace_110.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_110.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_110.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_110.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_110.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_110.Image = CType(resources.GetObject("_picFace_110.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_110, CType(110, Short))
        Me._picFace_110.Location = New System.Drawing.Point(552, 208)
        Me._picFace_110.Name = "_picFace_110"
        Me._picFace_110.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_110.Size = New System.Drawing.Size(41, 41)
        Me._picFace_110.TabIndex = 124
        Me._picFace_110.TabStop = False
        '
        '_picFace_109
        '
        Me._picFace_109.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_109.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_109.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_109.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_109.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_109.Image = CType(resources.GetObject("_picFace_109.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_109, CType(109, Short))
        Me._picFace_109.Location = New System.Drawing.Point(592, 208)
        Me._picFace_109.Name = "_picFace_109"
        Me._picFace_109.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_109.Size = New System.Drawing.Size(41, 41)
        Me._picFace_109.TabIndex = 123
        Me._picFace_109.TabStop = False
        '
        '_picFace_108
        '
        Me._picFace_108.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_108.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_108.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_108.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_108.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_108.Image = CType(resources.GetObject("_picFace_108.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_108, CType(108, Short))
        Me._picFace_108.Location = New System.Drawing.Point(632, 208)
        Me._picFace_108.Name = "_picFace_108"
        Me._picFace_108.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_108.Size = New System.Drawing.Size(41, 41)
        Me._picFace_108.TabIndex = 122
        Me._picFace_108.TabStop = False
        '
        '_picFace_107
        '
        Me._picFace_107.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_107.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_107.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_107.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_107.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_107.Image = CType(resources.GetObject("_picFace_107.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_107, CType(107, Short))
        Me._picFace_107.Location = New System.Drawing.Point(672, 208)
        Me._picFace_107.Name = "_picFace_107"
        Me._picFace_107.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_107.Size = New System.Drawing.Size(41, 41)
        Me._picFace_107.TabIndex = 121
        Me._picFace_107.TabStop = False
        '
        '_picFace_106
        '
        Me._picFace_106.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_106.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_106.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_106.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_106.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_106.Image = CType(resources.GetObject("_picFace_106.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_106, CType(106, Short))
        Me._picFace_106.Location = New System.Drawing.Point(712, 208)
        Me._picFace_106.Name = "_picFace_106"
        Me._picFace_106.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_106.Size = New System.Drawing.Size(41, 41)
        Me._picFace_106.TabIndex = 120
        Me._picFace_106.TabStop = False
        '
        '_picFace_105
        '
        Me._picFace_105.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_105.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_105.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_105.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_105.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_105.Image = CType(resources.GetObject("_picFace_105.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_105, CType(105, Short))
        Me._picFace_105.Location = New System.Drawing.Point(752, 208)
        Me._picFace_105.Name = "_picFace_105"
        Me._picFace_105.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_105.Size = New System.Drawing.Size(41, 41)
        Me._picFace_105.TabIndex = 119
        Me._picFace_105.TabStop = False
        '
        '_picFace_104
        '
        Me._picFace_104.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_104.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_104.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_104.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_104.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_104.Image = CType(resources.GetObject("_picFace_104.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_104, CType(104, Short))
        Me._picFace_104.Location = New System.Drawing.Point(792, 208)
        Me._picFace_104.Name = "_picFace_104"
        Me._picFace_104.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_104.Size = New System.Drawing.Size(41, 41)
        Me._picFace_104.TabIndex = 118
        Me._picFace_104.TabStop = False
        '
        '_picFace_103
        '
        Me._picFace_103.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_103.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_103.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_103.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_103.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_103.Image = CType(resources.GetObject("_picFace_103.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_103, CType(103, Short))
        Me._picFace_103.Location = New System.Drawing.Point(832, 208)
        Me._picFace_103.Name = "_picFace_103"
        Me._picFace_103.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_103.Size = New System.Drawing.Size(41, 41)
        Me._picFace_103.TabIndex = 117
        Me._picFace_103.TabStop = False
        '
        '_picFace_102
        '
        Me._picFace_102.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_102.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_102.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_102.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_102.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_102.Image = CType(resources.GetObject("_picFace_102.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_102, CType(102, Short))
        Me._picFace_102.Location = New System.Drawing.Point(872, 208)
        Me._picFace_102.Name = "_picFace_102"
        Me._picFace_102.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_102.Size = New System.Drawing.Size(41, 41)
        Me._picFace_102.TabIndex = 116
        Me._picFace_102.TabStop = False
        '
        '_picFace_101
        '
        Me._picFace_101.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_101.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_101.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_101.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_101.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_101.Image = CType(resources.GetObject("_picFace_101.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_101, CType(101, Short))
        Me._picFace_101.Location = New System.Drawing.Point(912, 208)
        Me._picFace_101.Name = "_picFace_101"
        Me._picFace_101.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_101.Size = New System.Drawing.Size(41, 41)
        Me._picFace_101.TabIndex = 115
        Me._picFace_101.TabStop = False
        '
        '_picFace_100
        '
        Me._picFace_100.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_100.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_100.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_100.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_100.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_100.Image = CType(resources.GetObject("_picFace_100.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_100, CType(100, Short))
        Me._picFace_100.Location = New System.Drawing.Point(952, 208)
        Me._picFace_100.Name = "_picFace_100"
        Me._picFace_100.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_100.Size = New System.Drawing.Size(41, 41)
        Me._picFace_100.TabIndex = 114
        Me._picFace_100.TabStop = False
        '
        'CommonDialog1
        '
        Me.CommonDialog1.Enabled = True
        Me.CommonDialog1.Location = New System.Drawing.Point(24, 328)
        Me.CommonDialog1.Name = "CommonDialog1"
        Me.CommonDialog1.OcxState = CType(resources.GetObject("CommonDialog1.OcxState"), System.Windows.Forms.AxHost.State)
        Me.CommonDialog1.Size = New System.Drawing.Size(32, 32)
        Me.CommonDialog1.TabIndex = 360
        '
        'cmdClear
        '
        Me.cmdClear.BackColor = System.Drawing.SystemColors.Control
        Me.cmdClear.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdClear.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdClear.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdClear.Location = New System.Drawing.Point(16, 248)
        Me.cmdClear.Name = "cmdClear"
        Me.cmdClear.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdClear.Size = New System.Drawing.Size(145, 33)
        Me.cmdClear.TabIndex = 113
        Me.cmdClear.Text = "Erase Face Memory"
        '
        '_cmdName_5
        '
        Me._cmdName_5.BackColor = System.Drawing.SystemColors.Control
        Me._cmdName_5.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdName_5.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdName_5.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdName.SetIndex(Me._cmdName_5, CType(5, Short))
        Me._cmdName_5.Location = New System.Drawing.Point(8, 184)
        Me._cmdName_5.Name = "_cmdName_5"
        Me._cmdName_5.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdName_5.Size = New System.Drawing.Size(17, 17)
        Me._cmdName_5.TabIndex = 111
        '
        '_cmdName_4
        '
        Me._cmdName_4.BackColor = System.Drawing.SystemColors.Control
        Me._cmdName_4.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdName_4.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdName_4.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdName.SetIndex(Me._cmdName_4, CType(4, Short))
        Me._cmdName_4.Location = New System.Drawing.Point(8, 160)
        Me._cmdName_4.Name = "_cmdName_4"
        Me._cmdName_4.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdName_4.Size = New System.Drawing.Size(17, 17)
        Me._cmdName_4.TabIndex = 110
        '
        '_cmdName_3
        '
        Me._cmdName_3.BackColor = System.Drawing.SystemColors.Control
        Me._cmdName_3.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdName_3.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdName_3.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdName.SetIndex(Me._cmdName_3, CType(3, Short))
        Me._cmdName_3.Location = New System.Drawing.Point(8, 136)
        Me._cmdName_3.Name = "_cmdName_3"
        Me._cmdName_3.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdName_3.Size = New System.Drawing.Size(17, 17)
        Me._cmdName_3.TabIndex = 109
        '
        '_cmdName_2
        '
        Me._cmdName_2.BackColor = System.Drawing.SystemColors.Control
        Me._cmdName_2.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdName_2.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdName_2.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdName.SetIndex(Me._cmdName_2, CType(2, Short))
        Me._cmdName_2.Location = New System.Drawing.Point(8, 112)
        Me._cmdName_2.Name = "_cmdName_2"
        Me._cmdName_2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdName_2.Size = New System.Drawing.Size(17, 17)
        Me._cmdName_2.TabIndex = 108
        '
        '_cmdName_1
        '
        Me._cmdName_1.BackColor = System.Drawing.SystemColors.Control
        Me._cmdName_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._cmdName_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._cmdName_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdName.SetIndex(Me._cmdName_1, CType(1, Short))
        Me._cmdName_1.Location = New System.Drawing.Point(8, 88)
        Me._cmdName_1.Name = "_cmdName_1"
        Me._cmdName_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._cmdName_1.Size = New System.Drawing.Size(17, 17)
        Me._cmdName_1.TabIndex = 107
        '
        '_txtName_5
        '
        Me._txtName_5.AcceptsReturn = True
        Me._txtName_5.AutoSize = False
        Me._txtName_5.BackColor = System.Drawing.SystemColors.Window
        Me._txtName_5.Cursor = System.Windows.Forms.Cursors.IBeam
        Me._txtName_5.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._txtName_5.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtName.SetIndex(Me._txtName_5, CType(5, Short))
        Me._txtName_5.Location = New System.Drawing.Point(24, 184)
        Me._txtName_5.MaxLength = 0
        Me._txtName_5.Name = "_txtName_5"
        Me._txtName_5.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._txtName_5.Size = New System.Drawing.Size(129, 19)
        Me._txtName_5.TabIndex = 105
        Me._txtName_5.Text = "person6"
        '
        '_txtName_4
        '
        Me._txtName_4.AcceptsReturn = True
        Me._txtName_4.AutoSize = False
        Me._txtName_4.BackColor = System.Drawing.SystemColors.Window
        Me._txtName_4.Cursor = System.Windows.Forms.Cursors.IBeam
        Me._txtName_4.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._txtName_4.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtName.SetIndex(Me._txtName_4, CType(4, Short))
        Me._txtName_4.Location = New System.Drawing.Point(24, 160)
        Me._txtName_4.MaxLength = 0
        Me._txtName_4.Name = "_txtName_4"
        Me._txtName_4.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._txtName_4.Size = New System.Drawing.Size(129, 19)
        Me._txtName_4.TabIndex = 104
        Me._txtName_4.Text = "person5"
        '
        '_txtName_3
        '
        Me._txtName_3.AcceptsReturn = True
        Me._txtName_3.AutoSize = False
        Me._txtName_3.BackColor = System.Drawing.SystemColors.Window
        Me._txtName_3.Cursor = System.Windows.Forms.Cursors.IBeam
        Me._txtName_3.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._txtName_3.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtName.SetIndex(Me._txtName_3, CType(3, Short))
        Me._txtName_3.Location = New System.Drawing.Point(24, 136)
        Me._txtName_3.MaxLength = 0
        Me._txtName_3.Name = "_txtName_3"
        Me._txtName_3.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._txtName_3.Size = New System.Drawing.Size(129, 19)
        Me._txtName_3.TabIndex = 103
        Me._txtName_3.Text = "person4"
        '
        '_txtName_2
        '
        Me._txtName_2.AcceptsReturn = True
        Me._txtName_2.AutoSize = False
        Me._txtName_2.BackColor = System.Drawing.SystemColors.Window
        Me._txtName_2.Cursor = System.Windows.Forms.Cursors.IBeam
        Me._txtName_2.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._txtName_2.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtName.SetIndex(Me._txtName_2, CType(2, Short))
        Me._txtName_2.Location = New System.Drawing.Point(24, 112)
        Me._txtName_2.MaxLength = 0
        Me._txtName_2.Name = "_txtName_2"
        Me._txtName_2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._txtName_2.Size = New System.Drawing.Size(129, 19)
        Me._txtName_2.TabIndex = 102
        Me._txtName_2.Text = "person3"
        '
        '_txtName_1
        '
        Me._txtName_1.AcceptsReturn = True
        Me._txtName_1.AutoSize = False
        Me._txtName_1.BackColor = System.Drawing.SystemColors.Window
        Me._txtName_1.Cursor = System.Windows.Forms.Cursors.IBeam
        Me._txtName_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._txtName_1.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtName.SetIndex(Me._txtName_1, CType(1, Short))
        Me._txtName_1.Location = New System.Drawing.Point(24, 88)
        Me._txtName_1.MaxLength = 0
        Me._txtName_1.Name = "_txtName_1"
        Me._txtName_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._txtName_1.Size = New System.Drawing.Size(129, 19)
        Me._txtName_1.TabIndex = 101
        Me._txtName_1.Text = "person2"
        '
        '_txtName_0
        '
        Me._txtName_0.AcceptsReturn = True
        Me._txtName_0.AutoSize = False
        Me._txtName_0.BackColor = System.Drawing.SystemColors.Window
        Me._txtName_0.Cursor = System.Windows.Forms.Cursors.IBeam
        Me._txtName_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._txtName_0.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtName.SetIndex(Me._txtName_0, CType(0, Short))
        Me._txtName_0.Location = New System.Drawing.Point(24, 64)
        Me._txtName_0.MaxLength = 0
        Me._txtName_0.Name = "_txtName_0"
        Me._txtName_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._txtName_0.Size = New System.Drawing.Size(129, 19)
        Me._txtName_0.TabIndex = 100
        Me._txtName_0.Text = "person1"
        '
        '_picFace_99
        '
        Me._picFace_99.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_99.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_99.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_99.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_99.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_99.Image = CType(resources.GetObject("_picFace_99.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_99, CType(99, Short))
        Me._picFace_99.Location = New System.Drawing.Point(952, 168)
        Me._picFace_99.Name = "_picFace_99"
        Me._picFace_99.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_99.Size = New System.Drawing.Size(41, 41)
        Me._picFace_99.TabIndex = 99
        Me._picFace_99.TabStop = False
        '
        '_picFace_98
        '
        Me._picFace_98.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_98.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_98.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_98.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_98.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_98.Image = CType(resources.GetObject("_picFace_98.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_98, CType(98, Short))
        Me._picFace_98.Location = New System.Drawing.Point(912, 168)
        Me._picFace_98.Name = "_picFace_98"
        Me._picFace_98.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_98.Size = New System.Drawing.Size(41, 41)
        Me._picFace_98.TabIndex = 98
        Me._picFace_98.TabStop = False
        '
        '_picFace_97
        '
        Me._picFace_97.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_97.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_97.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_97.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_97.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_97.Image = CType(resources.GetObject("_picFace_97.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_97, CType(97, Short))
        Me._picFace_97.Location = New System.Drawing.Point(872, 168)
        Me._picFace_97.Name = "_picFace_97"
        Me._picFace_97.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_97.Size = New System.Drawing.Size(41, 41)
        Me._picFace_97.TabIndex = 97
        Me._picFace_97.TabStop = False
        '
        '_picFace_96
        '
        Me._picFace_96.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_96.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_96.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_96.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_96.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_96.Image = CType(resources.GetObject("_picFace_96.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_96, CType(96, Short))
        Me._picFace_96.Location = New System.Drawing.Point(832, 168)
        Me._picFace_96.Name = "_picFace_96"
        Me._picFace_96.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_96.Size = New System.Drawing.Size(41, 41)
        Me._picFace_96.TabIndex = 96
        Me._picFace_96.TabStop = False
        '
        '_picFace_95
        '
        Me._picFace_95.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_95.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_95.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_95.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_95.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_95.Image = CType(resources.GetObject("_picFace_95.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_95, CType(95, Short))
        Me._picFace_95.Location = New System.Drawing.Point(792, 168)
        Me._picFace_95.Name = "_picFace_95"
        Me._picFace_95.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_95.Size = New System.Drawing.Size(41, 41)
        Me._picFace_95.TabIndex = 95
        Me._picFace_95.TabStop = False
        '
        '_picFace_94
        '
        Me._picFace_94.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_94.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_94.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_94.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_94.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_94.Image = CType(resources.GetObject("_picFace_94.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_94, CType(94, Short))
        Me._picFace_94.Location = New System.Drawing.Point(752, 168)
        Me._picFace_94.Name = "_picFace_94"
        Me._picFace_94.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_94.Size = New System.Drawing.Size(41, 41)
        Me._picFace_94.TabIndex = 94
        Me._picFace_94.TabStop = False
        '
        '_picFace_93
        '
        Me._picFace_93.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_93.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_93.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_93.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_93.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_93.Image = CType(resources.GetObject("_picFace_93.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_93, CType(93, Short))
        Me._picFace_93.Location = New System.Drawing.Point(712, 168)
        Me._picFace_93.Name = "_picFace_93"
        Me._picFace_93.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_93.Size = New System.Drawing.Size(41, 41)
        Me._picFace_93.TabIndex = 93
        Me._picFace_93.TabStop = False
        '
        '_picFace_92
        '
        Me._picFace_92.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_92.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_92.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_92.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_92.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_92.Image = CType(resources.GetObject("_picFace_92.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_92, CType(92, Short))
        Me._picFace_92.Location = New System.Drawing.Point(672, 168)
        Me._picFace_92.Name = "_picFace_92"
        Me._picFace_92.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_92.Size = New System.Drawing.Size(41, 41)
        Me._picFace_92.TabIndex = 92
        Me._picFace_92.TabStop = False
        '
        '_picFace_91
        '
        Me._picFace_91.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_91.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_91.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_91.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_91.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_91.Image = CType(resources.GetObject("_picFace_91.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_91, CType(91, Short))
        Me._picFace_91.Location = New System.Drawing.Point(632, 168)
        Me._picFace_91.Name = "_picFace_91"
        Me._picFace_91.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_91.Size = New System.Drawing.Size(41, 41)
        Me._picFace_91.TabIndex = 91
        Me._picFace_91.TabStop = False
        '
        '_picFace_90
        '
        Me._picFace_90.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_90.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_90.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_90.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_90.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_90.Image = CType(resources.GetObject("_picFace_90.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_90, CType(90, Short))
        Me._picFace_90.Location = New System.Drawing.Point(592, 168)
        Me._picFace_90.Name = "_picFace_90"
        Me._picFace_90.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_90.Size = New System.Drawing.Size(41, 41)
        Me._picFace_90.TabIndex = 90
        Me._picFace_90.TabStop = False
        '
        '_picFace_89
        '
        Me._picFace_89.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_89.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_89.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_89.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_89.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_89.Image = CType(resources.GetObject("_picFace_89.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_89, CType(89, Short))
        Me._picFace_89.Location = New System.Drawing.Point(552, 168)
        Me._picFace_89.Name = "_picFace_89"
        Me._picFace_89.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_89.Size = New System.Drawing.Size(41, 41)
        Me._picFace_89.TabIndex = 89
        Me._picFace_89.TabStop = False
        '
        '_picFace_88
        '
        Me._picFace_88.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_88.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_88.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_88.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_88.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_88.Image = CType(resources.GetObject("_picFace_88.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_88, CType(88, Short))
        Me._picFace_88.Location = New System.Drawing.Point(512, 168)
        Me._picFace_88.Name = "_picFace_88"
        Me._picFace_88.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_88.Size = New System.Drawing.Size(41, 41)
        Me._picFace_88.TabIndex = 88
        Me._picFace_88.TabStop = False
        '
        '_picFace_87
        '
        Me._picFace_87.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_87.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_87.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_87.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_87.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_87.Image = CType(resources.GetObject("_picFace_87.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_87, CType(87, Short))
        Me._picFace_87.Location = New System.Drawing.Point(472, 168)
        Me._picFace_87.Name = "_picFace_87"
        Me._picFace_87.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_87.Size = New System.Drawing.Size(41, 41)
        Me._picFace_87.TabIndex = 87
        Me._picFace_87.TabStop = False
        '
        '_picFace_86
        '
        Me._picFace_86.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_86.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_86.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_86.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_86.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_86.Image = CType(resources.GetObject("_picFace_86.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_86, CType(86, Short))
        Me._picFace_86.Location = New System.Drawing.Point(432, 168)
        Me._picFace_86.Name = "_picFace_86"
        Me._picFace_86.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_86.Size = New System.Drawing.Size(41, 41)
        Me._picFace_86.TabIndex = 86
        Me._picFace_86.TabStop = False
        '
        '_picFace_85
        '
        Me._picFace_85.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_85.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_85.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_85.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_85.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_85.Image = CType(resources.GetObject("_picFace_85.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_85, CType(85, Short))
        Me._picFace_85.Location = New System.Drawing.Point(392, 168)
        Me._picFace_85.Name = "_picFace_85"
        Me._picFace_85.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_85.Size = New System.Drawing.Size(41, 41)
        Me._picFace_85.TabIndex = 85
        Me._picFace_85.TabStop = False
        '
        '_picFace_84
        '
        Me._picFace_84.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_84.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_84.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_84.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_84.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_84.Image = CType(resources.GetObject("_picFace_84.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_84, CType(84, Short))
        Me._picFace_84.Location = New System.Drawing.Point(352, 168)
        Me._picFace_84.Name = "_picFace_84"
        Me._picFace_84.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_84.Size = New System.Drawing.Size(41, 41)
        Me._picFace_84.TabIndex = 84
        Me._picFace_84.TabStop = False
        '
        '_picFace_83
        '
        Me._picFace_83.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_83.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_83.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_83.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_83.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_83.Image = CType(resources.GetObject("_picFace_83.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_83, CType(83, Short))
        Me._picFace_83.Location = New System.Drawing.Point(312, 168)
        Me._picFace_83.Name = "_picFace_83"
        Me._picFace_83.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_83.Size = New System.Drawing.Size(41, 41)
        Me._picFace_83.TabIndex = 83
        Me._picFace_83.TabStop = False
        '
        '_picFace_82
        '
        Me._picFace_82.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_82.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_82.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_82.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_82.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_82.Image = CType(resources.GetObject("_picFace_82.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_82, CType(82, Short))
        Me._picFace_82.Location = New System.Drawing.Point(272, 168)
        Me._picFace_82.Name = "_picFace_82"
        Me._picFace_82.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_82.Size = New System.Drawing.Size(41, 41)
        Me._picFace_82.TabIndex = 82
        Me._picFace_82.TabStop = False
        '
        '_picFace_81
        '
        Me._picFace_81.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_81.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_81.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_81.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_81.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_81.Image = CType(resources.GetObject("_picFace_81.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_81, CType(81, Short))
        Me._picFace_81.Location = New System.Drawing.Point(232, 168)
        Me._picFace_81.Name = "_picFace_81"
        Me._picFace_81.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_81.Size = New System.Drawing.Size(41, 41)
        Me._picFace_81.TabIndex = 81
        Me._picFace_81.TabStop = False
        '
        '_picFace_80
        '
        Me._picFace_80.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_80.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_80.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_80.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_80.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_80.Image = CType(resources.GetObject("_picFace_80.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_80, CType(80, Short))
        Me._picFace_80.Location = New System.Drawing.Point(192, 168)
        Me._picFace_80.Name = "_picFace_80"
        Me._picFace_80.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_80.Size = New System.Drawing.Size(41, 41)
        Me._picFace_80.TabIndex = 80
        Me._picFace_80.TabStop = False
        '
        '_picFace_79
        '
        Me._picFace_79.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_79.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_79.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_79.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_79.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_79.Image = CType(resources.GetObject("_picFace_79.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_79, CType(79, Short))
        Me._picFace_79.Location = New System.Drawing.Point(952, 128)
        Me._picFace_79.Name = "_picFace_79"
        Me._picFace_79.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_79.Size = New System.Drawing.Size(41, 41)
        Me._picFace_79.TabIndex = 79
        Me._picFace_79.TabStop = False
        '
        '_picFace_78
        '
        Me._picFace_78.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_78.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_78.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_78.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_78.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_78.Image = CType(resources.GetObject("_picFace_78.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_78, CType(78, Short))
        Me._picFace_78.Location = New System.Drawing.Point(912, 128)
        Me._picFace_78.Name = "_picFace_78"
        Me._picFace_78.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_78.Size = New System.Drawing.Size(41, 41)
        Me._picFace_78.TabIndex = 78
        Me._picFace_78.TabStop = False
        '
        '_picFace_77
        '
        Me._picFace_77.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_77.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_77.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_77.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_77.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_77.Image = CType(resources.GetObject("_picFace_77.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_77, CType(77, Short))
        Me._picFace_77.Location = New System.Drawing.Point(872, 128)
        Me._picFace_77.Name = "_picFace_77"
        Me._picFace_77.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_77.Size = New System.Drawing.Size(41, 41)
        Me._picFace_77.TabIndex = 77
        Me._picFace_77.TabStop = False
        '
        '_picFace_76
        '
        Me._picFace_76.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_76.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_76.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_76.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_76.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_76.Image = CType(resources.GetObject("_picFace_76.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_76, CType(76, Short))
        Me._picFace_76.Location = New System.Drawing.Point(832, 128)
        Me._picFace_76.Name = "_picFace_76"
        Me._picFace_76.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_76.Size = New System.Drawing.Size(41, 41)
        Me._picFace_76.TabIndex = 76
        Me._picFace_76.TabStop = False
        '
        '_picFace_75
        '
        Me._picFace_75.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_75.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_75.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_75.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_75.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_75.Image = CType(resources.GetObject("_picFace_75.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_75, CType(75, Short))
        Me._picFace_75.Location = New System.Drawing.Point(792, 128)
        Me._picFace_75.Name = "_picFace_75"
        Me._picFace_75.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_75.Size = New System.Drawing.Size(41, 41)
        Me._picFace_75.TabIndex = 75
        Me._picFace_75.TabStop = False
        '
        '_picFace_74
        '
        Me._picFace_74.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_74.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_74.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_74.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_74.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_74.Image = CType(resources.GetObject("_picFace_74.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_74, CType(74, Short))
        Me._picFace_74.Location = New System.Drawing.Point(752, 128)
        Me._picFace_74.Name = "_picFace_74"
        Me._picFace_74.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_74.Size = New System.Drawing.Size(41, 41)
        Me._picFace_74.TabIndex = 74
        Me._picFace_74.TabStop = False
        '
        '_picFace_73
        '
        Me._picFace_73.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_73.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_73.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_73.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_73.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_73.Image = CType(resources.GetObject("_picFace_73.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_73, CType(73, Short))
        Me._picFace_73.Location = New System.Drawing.Point(712, 128)
        Me._picFace_73.Name = "_picFace_73"
        Me._picFace_73.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_73.Size = New System.Drawing.Size(41, 41)
        Me._picFace_73.TabIndex = 73
        Me._picFace_73.TabStop = False
        '
        '_picFace_72
        '
        Me._picFace_72.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_72.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_72.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_72.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_72.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_72.Image = CType(resources.GetObject("_picFace_72.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_72, CType(72, Short))
        Me._picFace_72.Location = New System.Drawing.Point(672, 128)
        Me._picFace_72.Name = "_picFace_72"
        Me._picFace_72.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_72.Size = New System.Drawing.Size(41, 41)
        Me._picFace_72.TabIndex = 72
        Me._picFace_72.TabStop = False
        '
        '_picFace_71
        '
        Me._picFace_71.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_71.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_71.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_71.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_71.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_71.Image = CType(resources.GetObject("_picFace_71.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_71, CType(71, Short))
        Me._picFace_71.Location = New System.Drawing.Point(632, 128)
        Me._picFace_71.Name = "_picFace_71"
        Me._picFace_71.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_71.Size = New System.Drawing.Size(41, 41)
        Me._picFace_71.TabIndex = 71
        Me._picFace_71.TabStop = False
        '
        '_picFace_70
        '
        Me._picFace_70.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_70.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_70.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_70.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_70.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_70.Image = CType(resources.GetObject("_picFace_70.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_70, CType(70, Short))
        Me._picFace_70.Location = New System.Drawing.Point(592, 128)
        Me._picFace_70.Name = "_picFace_70"
        Me._picFace_70.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_70.Size = New System.Drawing.Size(41, 41)
        Me._picFace_70.TabIndex = 70
        Me._picFace_70.TabStop = False
        '
        '_picFace_69
        '
        Me._picFace_69.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_69.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_69.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_69.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_69.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_69.Image = CType(resources.GetObject("_picFace_69.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_69, CType(69, Short))
        Me._picFace_69.Location = New System.Drawing.Point(552, 128)
        Me._picFace_69.Name = "_picFace_69"
        Me._picFace_69.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_69.Size = New System.Drawing.Size(41, 41)
        Me._picFace_69.TabIndex = 69
        Me._picFace_69.TabStop = False
        '
        '_picFace_68
        '
        Me._picFace_68.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_68.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_68.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_68.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_68.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_68.Image = CType(resources.GetObject("_picFace_68.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_68, CType(68, Short))
        Me._picFace_68.Location = New System.Drawing.Point(512, 128)
        Me._picFace_68.Name = "_picFace_68"
        Me._picFace_68.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_68.Size = New System.Drawing.Size(41, 41)
        Me._picFace_68.TabIndex = 68
        Me._picFace_68.TabStop = False
        '
        '_picFace_67
        '
        Me._picFace_67.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_67.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_67.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_67.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_67.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_67.Image = CType(resources.GetObject("_picFace_67.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_67, CType(67, Short))
        Me._picFace_67.Location = New System.Drawing.Point(472, 128)
        Me._picFace_67.Name = "_picFace_67"
        Me._picFace_67.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_67.Size = New System.Drawing.Size(41, 41)
        Me._picFace_67.TabIndex = 67
        Me._picFace_67.TabStop = False
        '
        '_picFace_66
        '
        Me._picFace_66.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_66.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_66.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_66.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_66.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_66.Image = CType(resources.GetObject("_picFace_66.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_66, CType(66, Short))
        Me._picFace_66.Location = New System.Drawing.Point(432, 128)
        Me._picFace_66.Name = "_picFace_66"
        Me._picFace_66.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_66.Size = New System.Drawing.Size(41, 41)
        Me._picFace_66.TabIndex = 66
        Me._picFace_66.TabStop = False
        '
        '_picFace_65
        '
        Me._picFace_65.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_65.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_65.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_65.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_65.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_65.Image = CType(resources.GetObject("_picFace_65.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_65, CType(65, Short))
        Me._picFace_65.Location = New System.Drawing.Point(392, 128)
        Me._picFace_65.Name = "_picFace_65"
        Me._picFace_65.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_65.Size = New System.Drawing.Size(41, 41)
        Me._picFace_65.TabIndex = 65
        Me._picFace_65.TabStop = False
        '
        '_picFace_64
        '
        Me._picFace_64.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_64.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_64.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_64.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_64.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_64.Image = CType(resources.GetObject("_picFace_64.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_64, CType(64, Short))
        Me._picFace_64.Location = New System.Drawing.Point(352, 128)
        Me._picFace_64.Name = "_picFace_64"
        Me._picFace_64.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_64.Size = New System.Drawing.Size(41, 41)
        Me._picFace_64.TabIndex = 64
        Me._picFace_64.TabStop = False
        '
        '_picFace_63
        '
        Me._picFace_63.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_63.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_63.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_63.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_63.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_63.Image = CType(resources.GetObject("_picFace_63.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_63, CType(63, Short))
        Me._picFace_63.Location = New System.Drawing.Point(312, 128)
        Me._picFace_63.Name = "_picFace_63"
        Me._picFace_63.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_63.Size = New System.Drawing.Size(41, 41)
        Me._picFace_63.TabIndex = 63
        Me._picFace_63.TabStop = False
        '
        '_picFace_62
        '
        Me._picFace_62.BackColor = System.Drawing.Color.White
        Me._picFace_62.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_62.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_62.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_62.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_62.Image = CType(resources.GetObject("_picFace_62.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_62, CType(62, Short))
        Me._picFace_62.Location = New System.Drawing.Point(272, 128)
        Me._picFace_62.Name = "_picFace_62"
        Me._picFace_62.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_62.Size = New System.Drawing.Size(41, 41)
        Me._picFace_62.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me._picFace_62.TabIndex = 62
        Me._picFace_62.TabStop = False
        '
        '_picFace_61
        '
        Me._picFace_61.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_61.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_61.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_61.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_61.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_61.Image = CType(resources.GetObject("_picFace_61.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_61, CType(61, Short))
        Me._picFace_61.Location = New System.Drawing.Point(232, 128)
        Me._picFace_61.Name = "_picFace_61"
        Me._picFace_61.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_61.Size = New System.Drawing.Size(41, 41)
        Me._picFace_61.TabIndex = 61
        Me._picFace_61.TabStop = False
        '
        '_picFace_60
        '
        Me._picFace_60.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_60.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_60.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_60.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_60.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_60.Image = CType(resources.GetObject("_picFace_60.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_60, CType(60, Short))
        Me._picFace_60.Location = New System.Drawing.Point(192, 128)
        Me._picFace_60.Name = "_picFace_60"
        Me._picFace_60.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_60.Size = New System.Drawing.Size(41, 41)
        Me._picFace_60.TabIndex = 60
        Me._picFace_60.TabStop = False
        '
        '_picFace_59
        '
        Me._picFace_59.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_59.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_59.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_59.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_59.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_59.Image = CType(resources.GetObject("_picFace_59.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_59, CType(59, Short))
        Me._picFace_59.Location = New System.Drawing.Point(952, 88)
        Me._picFace_59.Name = "_picFace_59"
        Me._picFace_59.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_59.Size = New System.Drawing.Size(41, 41)
        Me._picFace_59.TabIndex = 59
        Me._picFace_59.TabStop = False
        '
        '_picFace_58
        '
        Me._picFace_58.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_58.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_58.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_58.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_58.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_58.Image = CType(resources.GetObject("_picFace_58.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_58, CType(58, Short))
        Me._picFace_58.Location = New System.Drawing.Point(912, 88)
        Me._picFace_58.Name = "_picFace_58"
        Me._picFace_58.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_58.Size = New System.Drawing.Size(41, 41)
        Me._picFace_58.TabIndex = 58
        Me._picFace_58.TabStop = False
        '
        '_picFace_57
        '
        Me._picFace_57.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_57.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_57.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_57.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_57.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_57.Image = CType(resources.GetObject("_picFace_57.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_57, CType(57, Short))
        Me._picFace_57.Location = New System.Drawing.Point(872, 88)
        Me._picFace_57.Name = "_picFace_57"
        Me._picFace_57.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_57.Size = New System.Drawing.Size(41, 41)
        Me._picFace_57.TabIndex = 57
        Me._picFace_57.TabStop = False
        '
        '_picFace_56
        '
        Me._picFace_56.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_56.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_56.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_56.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_56.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_56.Image = CType(resources.GetObject("_picFace_56.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_56, CType(56, Short))
        Me._picFace_56.Location = New System.Drawing.Point(832, 88)
        Me._picFace_56.Name = "_picFace_56"
        Me._picFace_56.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_56.Size = New System.Drawing.Size(41, 41)
        Me._picFace_56.TabIndex = 56
        Me._picFace_56.TabStop = False
        '
        '_picFace_55
        '
        Me._picFace_55.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_55.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_55.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_55.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_55.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_55.Image = CType(resources.GetObject("_picFace_55.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_55, CType(55, Short))
        Me._picFace_55.Location = New System.Drawing.Point(792, 88)
        Me._picFace_55.Name = "_picFace_55"
        Me._picFace_55.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_55.Size = New System.Drawing.Size(41, 41)
        Me._picFace_55.TabIndex = 55
        Me._picFace_55.TabStop = False
        '
        '_picFace_54
        '
        Me._picFace_54.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_54.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_54.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_54.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_54.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_54.Image = CType(resources.GetObject("_picFace_54.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_54, CType(54, Short))
        Me._picFace_54.Location = New System.Drawing.Point(752, 88)
        Me._picFace_54.Name = "_picFace_54"
        Me._picFace_54.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_54.Size = New System.Drawing.Size(41, 41)
        Me._picFace_54.TabIndex = 54
        Me._picFace_54.TabStop = False
        '
        '_picFace_53
        '
        Me._picFace_53.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_53.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_53.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_53.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_53.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_53.Image = CType(resources.GetObject("_picFace_53.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_53, CType(53, Short))
        Me._picFace_53.Location = New System.Drawing.Point(712, 88)
        Me._picFace_53.Name = "_picFace_53"
        Me._picFace_53.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_53.Size = New System.Drawing.Size(41, 41)
        Me._picFace_53.TabIndex = 53
        Me._picFace_53.TabStop = False
        '
        '_picFace_52
        '
        Me._picFace_52.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_52.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_52.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_52.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_52.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_52.Image = CType(resources.GetObject("_picFace_52.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_52, CType(52, Short))
        Me._picFace_52.Location = New System.Drawing.Point(672, 88)
        Me._picFace_52.Name = "_picFace_52"
        Me._picFace_52.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_52.Size = New System.Drawing.Size(41, 41)
        Me._picFace_52.TabIndex = 52
        Me._picFace_52.TabStop = False
        '
        '_picFace_51
        '
        Me._picFace_51.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_51.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_51.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_51.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_51.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_51.Image = CType(resources.GetObject("_picFace_51.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_51, CType(51, Short))
        Me._picFace_51.Location = New System.Drawing.Point(632, 88)
        Me._picFace_51.Name = "_picFace_51"
        Me._picFace_51.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_51.Size = New System.Drawing.Size(41, 41)
        Me._picFace_51.TabIndex = 51
        Me._picFace_51.TabStop = False
        '
        '_picFace_50
        '
        Me._picFace_50.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_50.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_50.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_50.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_50.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_50.Image = CType(resources.GetObject("_picFace_50.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_50, CType(50, Short))
        Me._picFace_50.Location = New System.Drawing.Point(592, 88)
        Me._picFace_50.Name = "_picFace_50"
        Me._picFace_50.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_50.Size = New System.Drawing.Size(41, 41)
        Me._picFace_50.TabIndex = 50
        Me._picFace_50.TabStop = False
        '
        '_picFace_49
        '
        Me._picFace_49.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_49.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_49.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_49.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_49.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_49.Image = CType(resources.GetObject("_picFace_49.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_49, CType(49, Short))
        Me._picFace_49.Location = New System.Drawing.Point(552, 88)
        Me._picFace_49.Name = "_picFace_49"
        Me._picFace_49.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_49.Size = New System.Drawing.Size(41, 41)
        Me._picFace_49.TabIndex = 49
        Me._picFace_49.TabStop = False
        '
        '_picFace_48
        '
        Me._picFace_48.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_48.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_48.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_48.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_48.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_48.Image = CType(resources.GetObject("_picFace_48.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_48, CType(48, Short))
        Me._picFace_48.Location = New System.Drawing.Point(512, 88)
        Me._picFace_48.Name = "_picFace_48"
        Me._picFace_48.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_48.Size = New System.Drawing.Size(41, 41)
        Me._picFace_48.TabIndex = 48
        Me._picFace_48.TabStop = False
        '
        '_picFace_47
        '
        Me._picFace_47.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_47.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_47.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_47.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_47.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_47.Image = CType(resources.GetObject("_picFace_47.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_47, CType(47, Short))
        Me._picFace_47.Location = New System.Drawing.Point(472, 88)
        Me._picFace_47.Name = "_picFace_47"
        Me._picFace_47.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_47.Size = New System.Drawing.Size(41, 41)
        Me._picFace_47.TabIndex = 47
        Me._picFace_47.TabStop = False
        '
        '_picFace_46
        '
        Me._picFace_46.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_46.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_46.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_46.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_46.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_46.Image = CType(resources.GetObject("_picFace_46.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_46, CType(46, Short))
        Me._picFace_46.Location = New System.Drawing.Point(432, 88)
        Me._picFace_46.Name = "_picFace_46"
        Me._picFace_46.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_46.Size = New System.Drawing.Size(41, 41)
        Me._picFace_46.TabIndex = 46
        Me._picFace_46.TabStop = False
        '
        '_picFace_45
        '
        Me._picFace_45.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_45.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_45.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_45.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_45.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_45.Image = CType(resources.GetObject("_picFace_45.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_45, CType(45, Short))
        Me._picFace_45.Location = New System.Drawing.Point(392, 88)
        Me._picFace_45.Name = "_picFace_45"
        Me._picFace_45.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_45.Size = New System.Drawing.Size(41, 41)
        Me._picFace_45.TabIndex = 45
        Me._picFace_45.TabStop = False
        '
        '_picFace_44
        '
        Me._picFace_44.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_44.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_44.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_44.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_44.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_44.Image = CType(resources.GetObject("_picFace_44.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_44, CType(44, Short))
        Me._picFace_44.Location = New System.Drawing.Point(352, 88)
        Me._picFace_44.Name = "_picFace_44"
        Me._picFace_44.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_44.Size = New System.Drawing.Size(41, 41)
        Me._picFace_44.TabIndex = 44
        Me._picFace_44.TabStop = False
        '
        '_picFace_43
        '
        Me._picFace_43.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_43.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_43.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_43.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_43.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_43.Image = CType(resources.GetObject("_picFace_43.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_43, CType(43, Short))
        Me._picFace_43.Location = New System.Drawing.Point(312, 88)
        Me._picFace_43.Name = "_picFace_43"
        Me._picFace_43.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_43.Size = New System.Drawing.Size(41, 41)
        Me._picFace_43.TabIndex = 43
        Me._picFace_43.TabStop = False
        '
        '_picFace_42
        '
        Me._picFace_42.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_42.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_42.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_42.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_42.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_42.Image = CType(resources.GetObject("_picFace_42.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_42, CType(42, Short))
        Me._picFace_42.Location = New System.Drawing.Point(272, 88)
        Me._picFace_42.Name = "_picFace_42"
        Me._picFace_42.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_42.Size = New System.Drawing.Size(41, 41)
        Me._picFace_42.TabIndex = 42
        Me._picFace_42.TabStop = False
        '
        '_picFace_41
        '
        Me._picFace_41.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_41.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_41.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_41.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_41.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_41.Image = CType(resources.GetObject("_picFace_41.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_41, CType(41, Short))
        Me._picFace_41.Location = New System.Drawing.Point(232, 88)
        Me._picFace_41.Name = "_picFace_41"
        Me._picFace_41.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_41.Size = New System.Drawing.Size(41, 41)
        Me._picFace_41.TabIndex = 41
        Me._picFace_41.TabStop = False
        '
        '_picFace_40
        '
        Me._picFace_40.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_40.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_40.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_40.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_40.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_40.Image = CType(resources.GetObject("_picFace_40.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_40, CType(40, Short))
        Me._picFace_40.Location = New System.Drawing.Point(192, 88)
        Me._picFace_40.Name = "_picFace_40"
        Me._picFace_40.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_40.Size = New System.Drawing.Size(41, 41)
        Me._picFace_40.TabIndex = 40
        Me._picFace_40.TabStop = False
        '
        '_picFace_39
        '
        Me._picFace_39.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_39.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_39.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_39.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_39.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_39.Image = CType(resources.GetObject("_picFace_39.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_39, CType(39, Short))
        Me._picFace_39.Location = New System.Drawing.Point(952, 48)
        Me._picFace_39.Name = "_picFace_39"
        Me._picFace_39.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_39.Size = New System.Drawing.Size(41, 41)
        Me._picFace_39.TabIndex = 39
        Me._picFace_39.TabStop = False
        '
        '_picFace_38
        '
        Me._picFace_38.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_38.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_38.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_38.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_38.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_38.Image = CType(resources.GetObject("_picFace_38.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_38, CType(38, Short))
        Me._picFace_38.Location = New System.Drawing.Point(912, 48)
        Me._picFace_38.Name = "_picFace_38"
        Me._picFace_38.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_38.Size = New System.Drawing.Size(41, 41)
        Me._picFace_38.TabIndex = 38
        Me._picFace_38.TabStop = False
        '
        '_picFace_37
        '
        Me._picFace_37.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_37.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_37.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_37.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_37.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_37.Image = CType(resources.GetObject("_picFace_37.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_37, CType(37, Short))
        Me._picFace_37.Location = New System.Drawing.Point(872, 48)
        Me._picFace_37.Name = "_picFace_37"
        Me._picFace_37.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_37.Size = New System.Drawing.Size(41, 41)
        Me._picFace_37.TabIndex = 37
        Me._picFace_37.TabStop = False
        '
        '_picFace_36
        '
        Me._picFace_36.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_36.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_36.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_36.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_36.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_36.Image = CType(resources.GetObject("_picFace_36.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_36, CType(36, Short))
        Me._picFace_36.Location = New System.Drawing.Point(832, 48)
        Me._picFace_36.Name = "_picFace_36"
        Me._picFace_36.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_36.Size = New System.Drawing.Size(41, 41)
        Me._picFace_36.TabIndex = 36
        Me._picFace_36.TabStop = False
        '
        '_picFace_35
        '
        Me._picFace_35.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_35.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_35.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_35.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_35.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_35.Image = CType(resources.GetObject("_picFace_35.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_35, CType(35, Short))
        Me._picFace_35.Location = New System.Drawing.Point(792, 48)
        Me._picFace_35.Name = "_picFace_35"
        Me._picFace_35.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_35.Size = New System.Drawing.Size(41, 41)
        Me._picFace_35.TabIndex = 35
        Me._picFace_35.TabStop = False
        '
        '_picFace_34
        '
        Me._picFace_34.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_34.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_34.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_34.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_34.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_34.Image = CType(resources.GetObject("_picFace_34.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_34, CType(34, Short))
        Me._picFace_34.Location = New System.Drawing.Point(752, 48)
        Me._picFace_34.Name = "_picFace_34"
        Me._picFace_34.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_34.Size = New System.Drawing.Size(41, 41)
        Me._picFace_34.TabIndex = 34
        Me._picFace_34.TabStop = False
        '
        '_picFace_33
        '
        Me._picFace_33.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_33.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_33.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_33.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_33.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_33.Image = CType(resources.GetObject("_picFace_33.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_33, CType(33, Short))
        Me._picFace_33.Location = New System.Drawing.Point(712, 48)
        Me._picFace_33.Name = "_picFace_33"
        Me._picFace_33.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_33.Size = New System.Drawing.Size(41, 41)
        Me._picFace_33.TabIndex = 33
        Me._picFace_33.TabStop = False
        '
        '_picFace_32
        '
        Me._picFace_32.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_32.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_32.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_32.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_32.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_32.Image = CType(resources.GetObject("_picFace_32.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_32, CType(32, Short))
        Me._picFace_32.Location = New System.Drawing.Point(672, 48)
        Me._picFace_32.Name = "_picFace_32"
        Me._picFace_32.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_32.Size = New System.Drawing.Size(41, 41)
        Me._picFace_32.TabIndex = 32
        Me._picFace_32.TabStop = False
        '
        '_picFace_31
        '
        Me._picFace_31.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_31.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_31.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_31.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_31.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_31.Image = CType(resources.GetObject("_picFace_31.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_31, CType(31, Short))
        Me._picFace_31.Location = New System.Drawing.Point(632, 48)
        Me._picFace_31.Name = "_picFace_31"
        Me._picFace_31.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_31.Size = New System.Drawing.Size(41, 41)
        Me._picFace_31.TabIndex = 31
        Me._picFace_31.TabStop = False
        '
        '_picFace_30
        '
        Me._picFace_30.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_30.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_30.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_30.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_30.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_30.Image = CType(resources.GetObject("_picFace_30.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_30, CType(30, Short))
        Me._picFace_30.Location = New System.Drawing.Point(592, 48)
        Me._picFace_30.Name = "_picFace_30"
        Me._picFace_30.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_30.Size = New System.Drawing.Size(41, 41)
        Me._picFace_30.TabIndex = 30
        Me._picFace_30.TabStop = False
        '
        '_picFace_29
        '
        Me._picFace_29.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_29.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_29.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_29.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_29.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_29.Image = CType(resources.GetObject("_picFace_29.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_29, CType(29, Short))
        Me._picFace_29.Location = New System.Drawing.Point(552, 48)
        Me._picFace_29.Name = "_picFace_29"
        Me._picFace_29.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_29.Size = New System.Drawing.Size(41, 41)
        Me._picFace_29.TabIndex = 29
        Me._picFace_29.TabStop = False
        '
        '_picFace_28
        '
        Me._picFace_28.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_28.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_28.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_28.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_28.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_28.Image = CType(resources.GetObject("_picFace_28.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_28, CType(28, Short))
        Me._picFace_28.Location = New System.Drawing.Point(512, 48)
        Me._picFace_28.Name = "_picFace_28"
        Me._picFace_28.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_28.Size = New System.Drawing.Size(41, 41)
        Me._picFace_28.TabIndex = 28
        Me._picFace_28.TabStop = False
        '
        '_picFace_27
        '
        Me._picFace_27.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_27.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_27.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_27.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_27.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_27.Image = CType(resources.GetObject("_picFace_27.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_27, CType(27, Short))
        Me._picFace_27.Location = New System.Drawing.Point(472, 48)
        Me._picFace_27.Name = "_picFace_27"
        Me._picFace_27.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_27.Size = New System.Drawing.Size(41, 41)
        Me._picFace_27.TabIndex = 27
        Me._picFace_27.TabStop = False
        '
        '_picFace_26
        '
        Me._picFace_26.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_26.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_26.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_26.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_26.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_26.Image = CType(resources.GetObject("_picFace_26.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_26, CType(26, Short))
        Me._picFace_26.Location = New System.Drawing.Point(432, 48)
        Me._picFace_26.Name = "_picFace_26"
        Me._picFace_26.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_26.Size = New System.Drawing.Size(41, 41)
        Me._picFace_26.TabIndex = 26
        Me._picFace_26.TabStop = False
        '
        '_picFace_25
        '
        Me._picFace_25.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_25.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_25.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_25.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_25.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_25.Image = CType(resources.GetObject("_picFace_25.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_25, CType(25, Short))
        Me._picFace_25.Location = New System.Drawing.Point(392, 48)
        Me._picFace_25.Name = "_picFace_25"
        Me._picFace_25.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_25.Size = New System.Drawing.Size(41, 41)
        Me._picFace_25.TabIndex = 25
        Me._picFace_25.TabStop = False
        '
        '_picFace_24
        '
        Me._picFace_24.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_24.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_24.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_24.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_24.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_24.Image = CType(resources.GetObject("_picFace_24.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_24, CType(24, Short))
        Me._picFace_24.Location = New System.Drawing.Point(352, 48)
        Me._picFace_24.Name = "_picFace_24"
        Me._picFace_24.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_24.Size = New System.Drawing.Size(41, 41)
        Me._picFace_24.TabIndex = 24
        Me._picFace_24.TabStop = False
        '
        '_picFace_23
        '
        Me._picFace_23.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_23.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_23.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_23.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_23.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_23.Image = CType(resources.GetObject("_picFace_23.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_23, CType(23, Short))
        Me._picFace_23.Location = New System.Drawing.Point(312, 48)
        Me._picFace_23.Name = "_picFace_23"
        Me._picFace_23.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_23.Size = New System.Drawing.Size(41, 41)
        Me._picFace_23.TabIndex = 23
        Me._picFace_23.TabStop = False
        '
        '_picFace_22
        '
        Me._picFace_22.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_22.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_22.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_22.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_22.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_22.Image = CType(resources.GetObject("_picFace_22.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_22, CType(22, Short))
        Me._picFace_22.Location = New System.Drawing.Point(272, 48)
        Me._picFace_22.Name = "_picFace_22"
        Me._picFace_22.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_22.Size = New System.Drawing.Size(41, 41)
        Me._picFace_22.TabIndex = 22
        Me._picFace_22.TabStop = False
        '
        '_picFace_21
        '
        Me._picFace_21.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_21.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_21.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_21.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_21.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_21.Image = CType(resources.GetObject("_picFace_21.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_21, CType(21, Short))
        Me._picFace_21.Location = New System.Drawing.Point(232, 48)
        Me._picFace_21.Name = "_picFace_21"
        Me._picFace_21.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_21.Size = New System.Drawing.Size(41, 41)
        Me._picFace_21.TabIndex = 21
        Me._picFace_21.TabStop = False
        '
        '_picFace_20
        '
        Me._picFace_20.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_20.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_20.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_20.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_20.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_20.Image = CType(resources.GetObject("_picFace_20.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_20, CType(20, Short))
        Me._picFace_20.Location = New System.Drawing.Point(192, 48)
        Me._picFace_20.Name = "_picFace_20"
        Me._picFace_20.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_20.Size = New System.Drawing.Size(41, 41)
        Me._picFace_20.TabIndex = 20
        Me._picFace_20.TabStop = False
        '
        '_picFace_19
        '
        Me._picFace_19.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_19.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_19.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_19.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_19.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_19.Image = CType(resources.GetObject("_picFace_19.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_19, CType(19, Short))
        Me._picFace_19.Location = New System.Drawing.Point(952, 8)
        Me._picFace_19.Name = "_picFace_19"
        Me._picFace_19.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_19.Size = New System.Drawing.Size(41, 41)
        Me._picFace_19.TabIndex = 19
        Me._picFace_19.TabStop = False
        '
        '_picFace_18
        '
        Me._picFace_18.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_18.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_18.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_18.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_18.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_18.Image = CType(resources.GetObject("_picFace_18.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_18, CType(18, Short))
        Me._picFace_18.Location = New System.Drawing.Point(912, 8)
        Me._picFace_18.Name = "_picFace_18"
        Me._picFace_18.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_18.Size = New System.Drawing.Size(41, 41)
        Me._picFace_18.TabIndex = 18
        Me._picFace_18.TabStop = False
        '
        '_picFace_17
        '
        Me._picFace_17.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_17.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_17.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_17.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_17.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_17.Image = CType(resources.GetObject("_picFace_17.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_17, CType(17, Short))
        Me._picFace_17.Location = New System.Drawing.Point(872, 8)
        Me._picFace_17.Name = "_picFace_17"
        Me._picFace_17.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_17.Size = New System.Drawing.Size(41, 41)
        Me._picFace_17.TabIndex = 17
        Me._picFace_17.TabStop = False
        '
        '_picFace_16
        '
        Me._picFace_16.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_16.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_16.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_16.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_16.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_16.Image = CType(resources.GetObject("_picFace_16.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_16, CType(16, Short))
        Me._picFace_16.Location = New System.Drawing.Point(832, 8)
        Me._picFace_16.Name = "_picFace_16"
        Me._picFace_16.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_16.Size = New System.Drawing.Size(41, 41)
        Me._picFace_16.TabIndex = 16
        Me._picFace_16.TabStop = False
        '
        '_picFace_15
        '
        Me._picFace_15.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_15.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_15.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_15.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_15.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_15.Image = CType(resources.GetObject("_picFace_15.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_15, CType(15, Short))
        Me._picFace_15.Location = New System.Drawing.Point(792, 8)
        Me._picFace_15.Name = "_picFace_15"
        Me._picFace_15.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_15.Size = New System.Drawing.Size(41, 41)
        Me._picFace_15.TabIndex = 15
        Me._picFace_15.TabStop = False
        '
        '_picFace_14
        '
        Me._picFace_14.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_14.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_14.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_14.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_14.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_14.Image = CType(resources.GetObject("_picFace_14.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_14, CType(14, Short))
        Me._picFace_14.Location = New System.Drawing.Point(752, 8)
        Me._picFace_14.Name = "_picFace_14"
        Me._picFace_14.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_14.Size = New System.Drawing.Size(41, 41)
        Me._picFace_14.TabIndex = 14
        Me._picFace_14.TabStop = False
        '
        '_picFace_13
        '
        Me._picFace_13.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_13.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_13.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_13.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_13.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_13.Image = CType(resources.GetObject("_picFace_13.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_13, CType(13, Short))
        Me._picFace_13.Location = New System.Drawing.Point(712, 8)
        Me._picFace_13.Name = "_picFace_13"
        Me._picFace_13.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_13.Size = New System.Drawing.Size(41, 41)
        Me._picFace_13.TabIndex = 13
        Me._picFace_13.TabStop = False
        '
        '_picFace_12
        '
        Me._picFace_12.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_12.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_12.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_12.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_12.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_12.Image = CType(resources.GetObject("_picFace_12.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_12, CType(12, Short))
        Me._picFace_12.Location = New System.Drawing.Point(672, 8)
        Me._picFace_12.Name = "_picFace_12"
        Me._picFace_12.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_12.Size = New System.Drawing.Size(41, 41)
        Me._picFace_12.TabIndex = 12
        Me._picFace_12.TabStop = False
        '
        '_picFace_11
        '
        Me._picFace_11.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_11.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_11.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_11.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_11.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_11.Image = CType(resources.GetObject("_picFace_11.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_11, CType(11, Short))
        Me._picFace_11.Location = New System.Drawing.Point(632, 8)
        Me._picFace_11.Name = "_picFace_11"
        Me._picFace_11.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_11.Size = New System.Drawing.Size(41, 41)
        Me._picFace_11.TabIndex = 11
        Me._picFace_11.TabStop = False
        '
        '_picFace_10
        '
        Me._picFace_10.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_10.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_10.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_10.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_10.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_10.Image = CType(resources.GetObject("_picFace_10.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_10, CType(10, Short))
        Me._picFace_10.Location = New System.Drawing.Point(592, 8)
        Me._picFace_10.Name = "_picFace_10"
        Me._picFace_10.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_10.Size = New System.Drawing.Size(41, 41)
        Me._picFace_10.TabIndex = 10
        Me._picFace_10.TabStop = False
        '
        '_picFace_9
        '
        Me._picFace_9.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_9.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_9.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_9.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_9.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_9.Image = CType(resources.GetObject("_picFace_9.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_9, CType(9, Short))
        Me._picFace_9.Location = New System.Drawing.Point(552, 8)
        Me._picFace_9.Name = "_picFace_9"
        Me._picFace_9.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_9.Size = New System.Drawing.Size(41, 41)
        Me._picFace_9.TabIndex = 9
        Me._picFace_9.TabStop = False
        '
        '_picFace_8
        '
        Me._picFace_8.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_8.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_8.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_8.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_8.Image = CType(resources.GetObject("_picFace_8.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_8, CType(8, Short))
        Me._picFace_8.Location = New System.Drawing.Point(512, 8)
        Me._picFace_8.Name = "_picFace_8"
        Me._picFace_8.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_8.Size = New System.Drawing.Size(41, 41)
        Me._picFace_8.TabIndex = 8
        Me._picFace_8.TabStop = False
        '
        '_picFace_7
        '
        Me._picFace_7.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_7.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_7.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_7.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_7.Image = CType(resources.GetObject("_picFace_7.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_7, CType(7, Short))
        Me._picFace_7.Location = New System.Drawing.Point(472, 8)
        Me._picFace_7.Name = "_picFace_7"
        Me._picFace_7.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_7.Size = New System.Drawing.Size(41, 41)
        Me._picFace_7.TabIndex = 7
        Me._picFace_7.TabStop = False
        '
        '_picFace_6
        '
        Me._picFace_6.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_6.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_6.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_6.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_6.Image = CType(resources.GetObject("_picFace_6.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_6, CType(6, Short))
        Me._picFace_6.Location = New System.Drawing.Point(432, 8)
        Me._picFace_6.Name = "_picFace_6"
        Me._picFace_6.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_6.Size = New System.Drawing.Size(41, 41)
        Me._picFace_6.TabIndex = 6
        Me._picFace_6.TabStop = False
        '
        '_picFace_5
        '
        Me._picFace_5.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_5.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_5.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_5.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_5.Image = CType(resources.GetObject("_picFace_5.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_5, CType(5, Short))
        Me._picFace_5.Location = New System.Drawing.Point(392, 8)
        Me._picFace_5.Name = "_picFace_5"
        Me._picFace_5.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_5.Size = New System.Drawing.Size(41, 41)
        Me._picFace_5.TabIndex = 5
        Me._picFace_5.TabStop = False
        '
        '_picFace_4
        '
        Me._picFace_4.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_4.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_4.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_4.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_4.Image = CType(resources.GetObject("_picFace_4.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_4, CType(4, Short))
        Me._picFace_4.Location = New System.Drawing.Point(352, 8)
        Me._picFace_4.Name = "_picFace_4"
        Me._picFace_4.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_4.Size = New System.Drawing.Size(41, 41)
        Me._picFace_4.TabIndex = 4
        Me._picFace_4.TabStop = False
        '
        '_picFace_3
        '
        Me._picFace_3.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_3.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_3.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_3.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_3.Image = CType(resources.GetObject("_picFace_3.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_3, CType(3, Short))
        Me._picFace_3.Location = New System.Drawing.Point(312, 8)
        Me._picFace_3.Name = "_picFace_3"
        Me._picFace_3.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_3.Size = New System.Drawing.Size(41, 41)
        Me._picFace_3.TabIndex = 3
        Me._picFace_3.TabStop = False
        '
        '_picFace_2
        '
        Me._picFace_2.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_2.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_2.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_2.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_2.Image = CType(resources.GetObject("_picFace_2.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_2, CType(2, Short))
        Me._picFace_2.Location = New System.Drawing.Point(272, 8)
        Me._picFace_2.Name = "_picFace_2"
        Me._picFace_2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_2.Size = New System.Drawing.Size(41, 41)
        Me._picFace_2.TabIndex = 2
        Me._picFace_2.TabStop = False
        '
        '_picFace_1
        '
        Me._picFace_1.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_1.Image = CType(resources.GetObject("_picFace_1.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_1, CType(1, Short))
        Me._picFace_1.Location = New System.Drawing.Point(232, 8)
        Me._picFace_1.Name = "_picFace_1"
        Me._picFace_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_1.Size = New System.Drawing.Size(41, 41)
        Me._picFace_1.TabIndex = 1
        Me._picFace_1.TabStop = False
        '
        '_picFace_0
        '
        Me._picFace_0.BackColor = System.Drawing.SystemColors.Control
        Me._picFace_0.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me._picFace_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._picFace_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picFace_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picFace_0.Image = CType(resources.GetObject("_picFace_0.Image"), System.Drawing.Image)
        Me.picFace.SetIndex(Me._picFace_0, CType(0, Short))
        Me._picFace_0.Location = New System.Drawing.Point(192, 8)
        Me._picFace_0.Name = "_picFace_0"
        Me._picFace_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picFace_0.Size = New System.Drawing.Size(41, 41)
        Me._picFace_0.TabIndex = 0
        Me._picFace_0.TabStop = False
        '
        'imgPortrait
        '
        Me.imgPortrait.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.imgPortrait.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPortrait.Location = New System.Drawing.Point(8, 368)
        Me.imgPortrait.Name = "imgPortrait"
        Me.imgPortrait.Size = New System.Drawing.Size(177, 177)
        Me.imgPortrait.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.imgPortrait.TabIndex = 361
        Me.imgPortrait.TabStop = False
        '
        'Label1
        '
        Me.Label1.BackColor = System.Drawing.SystemColors.Control
        Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
        Me.Label1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Label1.Location = New System.Drawing.Point(8, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Label1.Size = New System.Drawing.Size(185, 49)
        Me.Label1.TabIndex = 112
        Me.Label1.Text = "Select the person from the list below, then click on the map to indicate the loca" & _
        "tion of identified faces"
        '
        'cmdName
        '
        '
        'cmdPicture
        '
        '
        'picFace
        '
        '
        'txtName
        '
        '
        'MainMenu1
        '
        Me.MainMenu1.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuFile})
        '
        'mnuFile
        '
        Me.mnuFile.Index = 0
        Me.mnuFile.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuOpen, Me.mnuSaveAs})
        Me.mnuFile.Text = "File"
        '
        'mnuOpen
        '
        Me.mnuOpen.Index = 0
        Me.mnuOpen.Text = "Open"
        '
        'mnuSaveAs
        '
        Me.mnuSaveAs.Index = 1
        Me.mnuSaveAs.Text = "Save As"
        '
        'frmMain
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.ClientSize = New System.Drawing.Size(1003, 714)
        Me.Controls.Add(Me._cmdPicture_5)
        Me.Controls.Add(Me._cmdPicture_4)
        Me.Controls.Add(Me._cmdPicture_3)
        Me.Controls.Add(Me._cmdPicture_2)
        Me.Controls.Add(Me._cmdPicture_1)
        Me.Controls.Add(Me._cmdPicture_0)
        Me.Controls.Add(Me._picFace_339)
        Me.Controls.Add(Me._picFace_338)
        Me.Controls.Add(Me._picFace_337)
        Me.Controls.Add(Me._picFace_336)
        Me.Controls.Add(Me._picFace_335)
        Me.Controls.Add(Me._picFace_334)
        Me.Controls.Add(Me._picFace_333)
        Me.Controls.Add(Me._picFace_332)
        Me.Controls.Add(Me._picFace_331)
        Me.Controls.Add(Me._picFace_330)
        Me.Controls.Add(Me._picFace_329)
        Me.Controls.Add(Me._picFace_328)
        Me.Controls.Add(Me._picFace_327)
        Me.Controls.Add(Me._picFace_326)
        Me.Controls.Add(Me._picFace_325)
        Me.Controls.Add(Me._picFace_324)
        Me.Controls.Add(Me._picFace_323)
        Me.Controls.Add(Me._picFace_322)
        Me.Controls.Add(Me._picFace_321)
        Me.Controls.Add(Me._picFace_320)
        Me.Controls.Add(Me._picFace_319)
        Me.Controls.Add(Me._picFace_318)
        Me.Controls.Add(Me._picFace_317)
        Me.Controls.Add(Me._picFace_316)
        Me.Controls.Add(Me._picFace_315)
        Me.Controls.Add(Me._picFace_314)
        Me.Controls.Add(Me._picFace_313)
        Me.Controls.Add(Me._picFace_312)
        Me.Controls.Add(Me._picFace_311)
        Me.Controls.Add(Me._picFace_310)
        Me.Controls.Add(Me._picFace_309)
        Me.Controls.Add(Me._picFace_308)
        Me.Controls.Add(Me._picFace_307)
        Me.Controls.Add(Me._picFace_306)
        Me.Controls.Add(Me._picFace_305)
        Me.Controls.Add(Me._picFace_304)
        Me.Controls.Add(Me._picFace_303)
        Me.Controls.Add(Me._picFace_302)
        Me.Controls.Add(Me._picFace_301)
        Me.Controls.Add(Me._picFace_300)
        Me.Controls.Add(Me._picFace_299)
        Me.Controls.Add(Me._picFace_298)
        Me.Controls.Add(Me._picFace_297)
        Me.Controls.Add(Me._picFace_296)
        Me.Controls.Add(Me._picFace_295)
        Me.Controls.Add(Me._picFace_294)
        Me.Controls.Add(Me._picFace_293)
        Me.Controls.Add(Me._picFace_292)
        Me.Controls.Add(Me._picFace_291)
        Me.Controls.Add(Me._picFace_290)
        Me.Controls.Add(Me._picFace_289)
        Me.Controls.Add(Me._picFace_288)
        Me.Controls.Add(Me._picFace_287)
        Me.Controls.Add(Me._picFace_286)
        Me.Controls.Add(Me._picFace_285)
        Me.Controls.Add(Me._picFace_284)
        Me.Controls.Add(Me._picFace_283)
        Me.Controls.Add(Me._picFace_282)
        Me.Controls.Add(Me._picFace_281)
        Me.Controls.Add(Me._picFace_280)
        Me.Controls.Add(Me._picFace_279)
        Me.Controls.Add(Me._picFace_278)
        Me.Controls.Add(Me._picFace_277)
        Me.Controls.Add(Me._picFace_276)
        Me.Controls.Add(Me._picFace_275)
        Me.Controls.Add(Me._picFace_274)
        Me.Controls.Add(Me._picFace_273)
        Me.Controls.Add(Me._picFace_272)
        Me.Controls.Add(Me._picFace_271)
        Me.Controls.Add(Me._picFace_270)
        Me.Controls.Add(Me._picFace_269)
        Me.Controls.Add(Me._picFace_268)
        Me.Controls.Add(Me._picFace_267)
        Me.Controls.Add(Me._picFace_266)
        Me.Controls.Add(Me._picFace_265)
        Me.Controls.Add(Me._picFace_264)
        Me.Controls.Add(Me._picFace_263)
        Me.Controls.Add(Me._picFace_262)
        Me.Controls.Add(Me._picFace_261)
        Me.Controls.Add(Me._picFace_260)
        Me.Controls.Add(Me._picFace_259)
        Me.Controls.Add(Me._picFace_258)
        Me.Controls.Add(Me._picFace_257)
        Me.Controls.Add(Me._picFace_256)
        Me.Controls.Add(Me._picFace_255)
        Me.Controls.Add(Me._picFace_254)
        Me.Controls.Add(Me._picFace_253)
        Me.Controls.Add(Me._picFace_252)
        Me.Controls.Add(Me._picFace_251)
        Me.Controls.Add(Me._picFace_250)
        Me.Controls.Add(Me._picFace_249)
        Me.Controls.Add(Me._picFace_248)
        Me.Controls.Add(Me._picFace_247)
        Me.Controls.Add(Me._picFace_246)
        Me.Controls.Add(Me._picFace_245)
        Me.Controls.Add(Me._picFace_244)
        Me.Controls.Add(Me._picFace_243)
        Me.Controls.Add(Me._picFace_242)
        Me.Controls.Add(Me._picFace_241)
        Me.Controls.Add(Me._picFace_240)
        Me.Controls.Add(Me._picFace_239)
        Me.Controls.Add(Me._picFace_238)
        Me.Controls.Add(Me._picFace_237)
        Me.Controls.Add(Me._picFace_236)
        Me.Controls.Add(Me._picFace_235)
        Me.Controls.Add(Me._picFace_234)
        Me.Controls.Add(Me._picFace_233)
        Me.Controls.Add(Me._picFace_232)
        Me.Controls.Add(Me._picFace_231)
        Me.Controls.Add(Me._picFace_230)
        Me.Controls.Add(Me._picFace_229)
        Me.Controls.Add(Me._picFace_228)
        Me.Controls.Add(Me._picFace_227)
        Me.Controls.Add(Me._picFace_226)
        Me.Controls.Add(Me._picFace_225)
        Me.Controls.Add(Me._picFace_224)
        Me.Controls.Add(Me._picFace_223)
        Me.Controls.Add(Me._picFace_222)
        Me.Controls.Add(Me._picFace_221)
        Me.Controls.Add(Me._picFace_220)
        Me.Controls.Add(Me._picFace_219)
        Me.Controls.Add(Me._picFace_218)
        Me.Controls.Add(Me._picFace_217)
        Me.Controls.Add(Me._picFace_216)
        Me.Controls.Add(Me._picFace_215)
        Me.Controls.Add(Me._picFace_214)
        Me.Controls.Add(Me._picFace_213)
        Me.Controls.Add(Me._picFace_212)
        Me.Controls.Add(Me._picFace_211)
        Me.Controls.Add(Me._picFace_210)
        Me.Controls.Add(Me._picFace_209)
        Me.Controls.Add(Me._picFace_208)
        Me.Controls.Add(Me._picFace_207)
        Me.Controls.Add(Me._picFace_206)
        Me.Controls.Add(Me._picFace_205)
        Me.Controls.Add(Me._picFace_204)
        Me.Controls.Add(Me._picFace_203)
        Me.Controls.Add(Me._picFace_202)
        Me.Controls.Add(Me._picFace_201)
        Me.Controls.Add(Me._picFace_200)
        Me.Controls.Add(Me._picFace_199)
        Me.Controls.Add(Me._picFace_198)
        Me.Controls.Add(Me._picFace_197)
        Me.Controls.Add(Me._picFace_196)
        Me.Controls.Add(Me._picFace_195)
        Me.Controls.Add(Me._picFace_194)
        Me.Controls.Add(Me._picFace_193)
        Me.Controls.Add(Me._picFace_192)
        Me.Controls.Add(Me._picFace_191)
        Me.Controls.Add(Me._picFace_190)
        Me.Controls.Add(Me._picFace_189)
        Me.Controls.Add(Me._picFace_188)
        Me.Controls.Add(Me._picFace_187)
        Me.Controls.Add(Me._picFace_186)
        Me.Controls.Add(Me._picFace_185)
        Me.Controls.Add(Me._picFace_184)
        Me.Controls.Add(Me._picFace_183)
        Me.Controls.Add(Me._picFace_182)
        Me.Controls.Add(Me._picFace_181)
        Me.Controls.Add(Me._picFace_180)
        Me.Controls.Add(Me._picFace_179)
        Me.Controls.Add(Me._picFace_178)
        Me.Controls.Add(Me._picFace_177)
        Me.Controls.Add(Me._picFace_176)
        Me.Controls.Add(Me._picFace_175)
        Me.Controls.Add(Me._picFace_174)
        Me.Controls.Add(Me._picFace_173)
        Me.Controls.Add(Me._picFace_172)
        Me.Controls.Add(Me._picFace_171)
        Me.Controls.Add(Me._picFace_170)
        Me.Controls.Add(Me._picFace_169)
        Me.Controls.Add(Me._picFace_168)
        Me.Controls.Add(Me._picFace_167)
        Me.Controls.Add(Me._picFace_166)
        Me.Controls.Add(Me._picFace_165)
        Me.Controls.Add(Me._picFace_164)
        Me.Controls.Add(Me._picFace_163)
        Me.Controls.Add(Me._picFace_162)
        Me.Controls.Add(Me._picFace_161)
        Me.Controls.Add(Me._picFace_160)
        Me.Controls.Add(Me._picFace_159)
        Me.Controls.Add(Me._picFace_158)
        Me.Controls.Add(Me._picFace_157)
        Me.Controls.Add(Me._picFace_156)
        Me.Controls.Add(Me._picFace_155)
        Me.Controls.Add(Me._picFace_154)
        Me.Controls.Add(Me._picFace_153)
        Me.Controls.Add(Me._picFace_152)
        Me.Controls.Add(Me._picFace_151)
        Me.Controls.Add(Me._picFace_150)
        Me.Controls.Add(Me._picFace_149)
        Me.Controls.Add(Me._picFace_148)
        Me.Controls.Add(Me._picFace_147)
        Me.Controls.Add(Me._picFace_146)
        Me.Controls.Add(Me._picFace_145)
        Me.Controls.Add(Me._picFace_144)
        Me.Controls.Add(Me._picFace_143)
        Me.Controls.Add(Me._picFace_142)
        Me.Controls.Add(Me._picFace_141)
        Me.Controls.Add(Me._picFace_140)
        Me.Controls.Add(Me._picFace_139)
        Me.Controls.Add(Me._picFace_138)
        Me.Controls.Add(Me._picFace_137)
        Me.Controls.Add(Me._picFace_136)
        Me.Controls.Add(Me._picFace_135)
        Me.Controls.Add(Me._picFace_134)
        Me.Controls.Add(Me._picFace_133)
        Me.Controls.Add(Me._picFace_132)
        Me.Controls.Add(Me._picFace_131)
        Me.Controls.Add(Me._picFace_130)
        Me.Controls.Add(Me._picFace_129)
        Me.Controls.Add(Me._picFace_128)
        Me.Controls.Add(Me._picFace_127)
        Me.Controls.Add(Me._picFace_126)
        Me.Controls.Add(Me._picFace_125)
        Me.Controls.Add(Me._picFace_124)
        Me.Controls.Add(Me._picFace_123)
        Me.Controls.Add(Me._picFace_122)
        Me.Controls.Add(Me._picFace_121)
        Me.Controls.Add(Me._picFace_120)
        Me.Controls.Add(Me._picFace_119)
        Me.Controls.Add(Me._picFace_118)
        Me.Controls.Add(Me._picFace_117)
        Me.Controls.Add(Me._picFace_116)
        Me.Controls.Add(Me._picFace_115)
        Me.Controls.Add(Me._picFace_114)
        Me.Controls.Add(Me._picFace_113)
        Me.Controls.Add(Me._picFace_112)
        Me.Controls.Add(Me._picFace_111)
        Me.Controls.Add(Me._picFace_110)
        Me.Controls.Add(Me._picFace_109)
        Me.Controls.Add(Me._picFace_108)
        Me.Controls.Add(Me._picFace_107)
        Me.Controls.Add(Me._picFace_106)
        Me.Controls.Add(Me._picFace_105)
        Me.Controls.Add(Me._picFace_104)
        Me.Controls.Add(Me._picFace_103)
        Me.Controls.Add(Me._picFace_102)
        Me.Controls.Add(Me._picFace_101)
        Me.Controls.Add(Me._picFace_100)
        Me.Controls.Add(Me.CommonDialog1)
        Me.Controls.Add(Me.cmdClear)
        Me.Controls.Add(Me._cmdName_5)
        Me.Controls.Add(Me._cmdName_4)
        Me.Controls.Add(Me._cmdName_3)
        Me.Controls.Add(Me._cmdName_2)
        Me.Controls.Add(Me._cmdName_1)
        Me.Controls.Add(Me._cmdName_0)
        Me.Controls.Add(Me._txtName_5)
        Me.Controls.Add(Me._txtName_4)
        Me.Controls.Add(Me._txtName_3)
        Me.Controls.Add(Me._txtName_2)
        Me.Controls.Add(Me._txtName_1)
        Me.Controls.Add(Me._txtName_0)
        Me.Controls.Add(Me._picFace_99)
        Me.Controls.Add(Me._picFace_98)
        Me.Controls.Add(Me._picFace_97)
        Me.Controls.Add(Me._picFace_96)
        Me.Controls.Add(Me._picFace_95)
        Me.Controls.Add(Me._picFace_94)
        Me.Controls.Add(Me._picFace_93)
        Me.Controls.Add(Me._picFace_92)
        Me.Controls.Add(Me._picFace_91)
        Me.Controls.Add(Me._picFace_90)
        Me.Controls.Add(Me._picFace_89)
        Me.Controls.Add(Me._picFace_88)
        Me.Controls.Add(Me._picFace_87)
        Me.Controls.Add(Me._picFace_86)
        Me.Controls.Add(Me._picFace_85)
        Me.Controls.Add(Me._picFace_84)
        Me.Controls.Add(Me._picFace_83)
        Me.Controls.Add(Me._picFace_82)
        Me.Controls.Add(Me._picFace_81)
        Me.Controls.Add(Me._picFace_80)
        Me.Controls.Add(Me._picFace_79)
        Me.Controls.Add(Me._picFace_78)
        Me.Controls.Add(Me._picFace_77)
        Me.Controls.Add(Me._picFace_76)
        Me.Controls.Add(Me._picFace_75)
        Me.Controls.Add(Me._picFace_74)
        Me.Controls.Add(Me._picFace_73)
        Me.Controls.Add(Me._picFace_72)
        Me.Controls.Add(Me._picFace_71)
        Me.Controls.Add(Me._picFace_70)
        Me.Controls.Add(Me._picFace_69)
        Me.Controls.Add(Me._picFace_68)
        Me.Controls.Add(Me._picFace_67)
        Me.Controls.Add(Me._picFace_66)
        Me.Controls.Add(Me._picFace_65)
        Me.Controls.Add(Me._picFace_64)
        Me.Controls.Add(Me._picFace_63)
        Me.Controls.Add(Me._picFace_62)
        Me.Controls.Add(Me._picFace_61)
        Me.Controls.Add(Me._picFace_60)
        Me.Controls.Add(Me._picFace_59)
        Me.Controls.Add(Me._picFace_58)
        Me.Controls.Add(Me._picFace_57)
        Me.Controls.Add(Me._picFace_56)
        Me.Controls.Add(Me._picFace_55)
        Me.Controls.Add(Me._picFace_54)
        Me.Controls.Add(Me._picFace_53)
        Me.Controls.Add(Me._picFace_52)
        Me.Controls.Add(Me._picFace_51)
        Me.Controls.Add(Me._picFace_50)
        Me.Controls.Add(Me._picFace_49)
        Me.Controls.Add(Me._picFace_48)
        Me.Controls.Add(Me._picFace_47)
        Me.Controls.Add(Me._picFace_46)
        Me.Controls.Add(Me._picFace_45)
        Me.Controls.Add(Me._picFace_44)
        Me.Controls.Add(Me._picFace_43)
        Me.Controls.Add(Me._picFace_42)
        Me.Controls.Add(Me._picFace_41)
        Me.Controls.Add(Me._picFace_40)
        Me.Controls.Add(Me._picFace_39)
        Me.Controls.Add(Me._picFace_38)
        Me.Controls.Add(Me._picFace_37)
        Me.Controls.Add(Me._picFace_36)
        Me.Controls.Add(Me._picFace_35)
        Me.Controls.Add(Me._picFace_34)
        Me.Controls.Add(Me._picFace_33)
        Me.Controls.Add(Me._picFace_32)
        Me.Controls.Add(Me._picFace_31)
        Me.Controls.Add(Me._picFace_30)
        Me.Controls.Add(Me._picFace_29)
        Me.Controls.Add(Me._picFace_28)
        Me.Controls.Add(Me._picFace_27)
        Me.Controls.Add(Me._picFace_26)
        Me.Controls.Add(Me._picFace_25)
        Me.Controls.Add(Me._picFace_24)
        Me.Controls.Add(Me._picFace_23)
        Me.Controls.Add(Me._picFace_22)
        Me.Controls.Add(Me._picFace_21)
        Me.Controls.Add(Me._picFace_20)
        Me.Controls.Add(Me._picFace_19)
        Me.Controls.Add(Me._picFace_18)
        Me.Controls.Add(Me._picFace_17)
        Me.Controls.Add(Me._picFace_16)
        Me.Controls.Add(Me._picFace_15)
        Me.Controls.Add(Me._picFace_14)
        Me.Controls.Add(Me._picFace_13)
        Me.Controls.Add(Me._picFace_12)
        Me.Controls.Add(Me._picFace_11)
        Me.Controls.Add(Me._picFace_10)
        Me.Controls.Add(Me._picFace_9)
        Me.Controls.Add(Me._picFace_8)
        Me.Controls.Add(Me._picFace_7)
        Me.Controls.Add(Me._picFace_6)
        Me.Controls.Add(Me._picFace_5)
        Me.Controls.Add(Me._picFace_4)
        Me.Controls.Add(Me._picFace_3)
        Me.Controls.Add(Me._picFace_2)
        Me.Controls.Add(Me._picFace_1)
        Me.Controls.Add(Me._picFace_0)
        Me.Controls.Add(Me.imgPortrait)
        Me.Controls.Add(Me.Label1)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Location = New System.Drawing.Point(11, 57)
        Me.Menu = Me.MainMenu1
        Me.Name = "frmMain"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Text = "Face memory browser"
        CType(Me.CommonDialog1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.cmdName, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.cmdPicture, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.picFace, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.txtName, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
#End Region
#Region "Upgrade Support "
    Private Shared m_vb6FormDefInstance As frmMain
    Private Shared m_InitializingDefInstance As Boolean
    Public Shared Property DefInstance() As frmMain
        Get
            If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
                m_InitializingDefInstance = True
                m_vb6FormDefInstance = New frmMain
                m_InitializingDefInstance = False
            End If
            DefInstance = m_vb6FormDefInstance
        End Get
        Set(ByVal Value As frmMain)
            m_vb6FormDefInstance = Value
        End Set
    End Property
#End Region

#Region "Local variables"

    'dimensions of the stereo image
    Dim IMAGE_WIDTH As Integer
    Dim IMAGE_HEIGHT As Integer



    Dim initialised As Boolean
    Dim sourceImage As ClassImage

    Dim topmap As ClassTopMap
    Dim TOPMAP_WIDTH As Short
    Dim TOPMAP_HEIGHT As Short
    Dim currentClassificationID As Short
    Dim Portrait(20) As String 'stores filenames of pictures for each person

#End Region


    ''' <summary>
    ''' Erase all learned information
    ''' </summary>
    Private Sub cmdClear_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdClear.Click
        Dim choice As Short
        Dim x As Short
        Dim y As Short

        choice = MsgBox("Are you sure that you want the system to forget all faces?", 4, "Erase face memory")
        If (choice = 6) Then
            Call topmap.initWeights()
            For x = 0 To TOPMAP_WIDTH - 1
                For y = 0 To TOPMAP_HEIGHT - 1
                    topmap.WinnerX = x
                    topmap.WinnerY = y
                    Call topmap.setClassification(0)
                Next
            Next
            Call Update_Map()
        End If
    End Sub

    ''' <summary>
    ''' Show the representitive units on the topological map corresponding to the selected person
    ''' </summary>
    Private Sub cmdName_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdName.Click
        Dim index As Short = cmdName.GetIndex(eventSender)
        currentClassificationID = index + 1
        Call showCategory(index + 1)
    End Sub

    ''' <summary>
    ''' Add a JPEG image for the selected person
    ''' </summary>
    Private Sub cmdPicture_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdPicture.Click
        Dim index As Short = cmdPicture.GetIndex(eventSender)

        If (txtName(index).Text <> "") Then
            CommonDialog1.DialogTitle = "Add a picture of this person"
            CommonDialog1.ShowOpen()
            CommonDialog1.Filter = "*.jpg"
            If (CommonDialog1.FileName <> "") Then
                If (InStr(UCase(CommonDialog1.FileName), ".JPG") > 0) Then
                    Portrait(index + 1) = CommonDialog1.FileName
                    Call LoadPortrait(index + 1)
                Else
                    MsgBox("Please select a JPG file")
                End If
            End If

        Else

            MsgBox("Please enter a name for the person first")
            txtName(index).Focus()
        End If
    End Sub

    ''' <summary>
    ''' Displays a JPEG image for the given person
    ''' </summary>
    ''' <param name="personID">A unique number greater than 1 corresponding to the person</param>
    Private Sub LoadPortrait(ByRef personID As Short)
        If (InStr(UCase(Portrait(personID)), ".JPG") > 0) Then
            imgPortrait.Image = System.Drawing.Image.FromFile(Portrait(personID))
        End If
    End Sub


    ''' <summary>
    ''' Load the main form and initialise the topological map
    ''' </summary>
    ''' <param name="personID">A unique number greater than 1 corresponding to the person</param>
    Private Sub frmMain_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
        IMAGE_WIDTH = 30
        IMAGE_HEIGHT = 30
        Call init()
        Call Update_Map()
    End Sub


    ''' <summary>
    ''' Load topological map data from file
    ''' </summary>
    ''' <param name="filename">The file name to load (typically "faces.net")</param>
    Private Sub loadData(ByRef filename As String)
        Dim i As Short
        Dim j As Short
        Dim className As String

        Call topmap.Load(filename)
        For i = 1 To 6
            className = topmap.getClassificationName(i)
            txtName(i - 1).Text = ""
            For j = 1 To Len(className)
                If (Asc(Mid(className, j, 1)) <> 13) And (Asc(Mid(className, j, 1)) <> 10) Then
                    txtName(i - 1).Text = txtName(i - 1).Text & Mid(className, j, 1)
                End If
            Next
        Next
    End Sub

    ''' <summary>
    ''' Initialise the topological map and load data from file
    ''' </summary>
    Private Sub init()
        Dim i As Short

        TOPMAP_WIDTH = 20
        TOPMAP_HEIGHT = 17

        topmap = New ClassTopMap
        Call topmap.init(CShort(IMAGE_WIDTH), CShort(IMAGE_HEIGHT), TOPMAP_WIDTH, TOPMAP_HEIGHT)

        Call topmap.setClassificationColour(1, 255, 0, 0)
        Call topmap.setClassificationColour(2, 0, 255, 0)
        Call topmap.setClassificationColour(3, 0, 0, 255)
        Call topmap.setClassificationColour(4, 255, 0, 255)
        Call topmap.setClassificationColour(5, 0, 255, 255)

        Call loadData("c:\develop\hbp\faces.net")

        For i = 0 To 19
            Portrait(i) = topmap.getPortrait(i)
        Next

        sourceImage = New ClassImage
        Call sourceImage.init(CShort(IMAGE_WIDTH), CShort(IMAGE_HEIGHT))
        initialised = True
    End Sub


    ''' <summary>
    ''' Alter the border style of pictures to show the representitive units for the currently selected person
    ''' </summary>
    Private Sub Update_Map()
        Dim x As Short
        Dim y As Short
        Dim i As Short

        i = 0
        For y = 0 To TOPMAP_HEIGHT - 1
            For x = 0 To TOPMAP_WIDTH - 1
                Call topmap.setImageWeights(x, y, sourceImage)
                Call sourceImage.Show(picFace(i))
                picFace(i).BorderStyle = System.Windows.Forms.BorderStyle.None
                i = i + 1
            Next
        Next
    End Sub

    ''' <summary>
    ''' Saves topological map data to file
    ''' </summary>
    ''' <param name="filename">The file name to save (typically "faces.net")</param>
    Private Sub saveData(ByRef filename As String)
        Dim i As Short

        For i = 1 To 6
            Call topmap.setClassificationName(i, Trim(txtName(i - 1).Text) & Chr(13))
            Call topmap.setPortrait(i, Portrait(i))
        Next

        Call topmap.Save(filename)
    End Sub


    ''' <summary>
    ''' Close the main form and save anything which needs saving
    ''' </summary>
    Private Sub frmMain_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
        Call saveData("c:\develop\hbp\faces.net")
    End Sub


    Public Sub mnuOpen_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOpen.Popup
        mnuOpen_Click(eventSender, eventArgs)
    End Sub

    ''' <summary>
    ''' Open a topological map data file from the menu
    ''' </summary>
    Public Sub mnuOpen_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOpen.Click
        CommonDialog1.Filter = "*.net"
        CommonDialog1.DialogTitle = "Open face data file"
        CommonDialog1.ShowOpen()
        If (CommonDialog1.FileName <> "") Then
            If (VB.Right(CommonDialog1.FileName, 4) = ".net") Then
                Call loadData((CommonDialog1.FileName))
                Call Update_Map()
            Else
                MsgBox("This is not a valid face data file")
            End If
        End If
    End Sub

    Public Sub mnuSaveAs_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuSaveAs.Popup
        mnuSaveAs_Click(eventSender, eventArgs)
    End Sub

    ''' <summary>
    ''' Save topological map data from the menu bar
    ''' </summary>
    Public Sub mnuSaveAs_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuSaveAs.Click
        Dim filename As String

        CommonDialog1.Filter = "*.net"
        CommonDialog1.DialogTitle = "Save face data file"
        CommonDialog1.ShowSave()
        If (CommonDialog1.FileName <> "") Then
            filename = CommonDialog1.FileName
            If (VB.Right(filename, 4) <> ".net") Then
                filename = filename & ".net"
            End If
            Call saveData(filename)
        End If
    End Sub

    ''' <summary>
    ''' Selects a unit on the topological map
    ''' </summary>
    Private Sub picFace_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles picFace.Click
        Dim index As Short = picFace.GetIndex(eventSender)
        Dim x As Short
        Dim y As Short
        Dim i As Short

        If (currentClassificationID > 0) Then
            i = 0
            For y = 0 To TOPMAP_HEIGHT - 1
                For x = 0 To TOPMAP_WIDTH - 1
                    If (i = index) Then
                        topmap.WinnerX = x
                        topmap.WinnerY = y
                        If (picFace(i).BorderStyle = 0) Then
                            picFace(i).BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
                            Call topmap.setClassification(0)
                        Else
                            picFace(i).BorderStyle = System.Windows.Forms.BorderStyle.None
                            Call topmap.setClassification(currentClassificationID)
                        End If
                    End If
                    i = i + 1
                Next
            Next
        End If
    End Sub


    ''' <summary>
    ''' Show representitive units on the topological map for the currently selected person
    ''' </summary>
    ''' <param name="ID">A unique number corresponding to the person</param>
    Private Sub showCategory(ByRef ID As Short)
        Dim x As Short
        Dim y As Short
        Dim i As Short

        i = 0
        For y = 0 To TOPMAP_HEIGHT - 1
            For x = 0 To TOPMAP_WIDTH - 1
                If (topmap.getClassification(x, y) = ID) Then
                    picFace(i).BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
                Else
                    picFace(i).BorderStyle = System.Windows.Forms.BorderStyle.None
                End If
                i = i + 1
            Next
        Next
    End Sub

    ''' <summary>
    ''' Shows an image of the person when you click on their name
    ''' </summary>
    Private Sub txtName_Enter(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles txtName.Enter
        Dim index As Short = txtName.GetIndex(eventSender)
        Call LoadPortrait(index + 1)
    End Sub

End Class