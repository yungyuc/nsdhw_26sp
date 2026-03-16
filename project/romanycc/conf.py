# DotArena Sphinx Configuration
master_doc = 'README'  # 注意：不要加 .rst 副檔名
# -- Project information -----------------------------------------------------
project = 'DotArena'
copyright = '2026, Roman'
author = 'Roman'
release = '0.1.0'

# -- General configuration ---------------------------------------------------
# 這裡加入必要的擴充功能
extensions = [
    'sphinx.ext.mathjax',      # 渲染數學公式（例如 :math:`m_1v_1`）
    'sphinx.ext.viewcode',     # 方便查看原始碼
    'sphinx.ext.todo',         # 支援 TODO 標記
]

# 排除不需要掃描的資料夾
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'venv', '.git']

# -- Options for HTML output -------------------------------------------------
# 使用經典主題，確保預覽相容性
html_theme = 'alabaster'

# 如果你想要數學公式顯示得更漂亮，可以設定 MathJax 路徑（選配）
mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js"