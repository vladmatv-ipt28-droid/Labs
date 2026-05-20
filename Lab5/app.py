import streamlit as st
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import urllib.request
import os
import time
from datetime import datetime

REGIONS = {
    1: "Вінницька", 2: "Волинська", 3: "Дніпропетровська", 4: "Донецька",
    5: "Житомирська", 6: "Закарпатська", 7: "Запорізька", 8: "Івано-Франківська",
    9: "Київська", 10: "Кіровоградська", 11: "Луганська", 12: "Львівська",
    13: "Миколаївська", 14: "Одеська", 15: "Полтавська", 16: "Рівненська",
    17: "Сумська", 18: "Тернопільська", 19: "Харківська", 20: "Херсонська",
    21: "Хмельницька", 22: "Черкаська", 23: "Чернівецька", 24: "Чернігівська"
}

UKR_TO_NOAA = {
    1: 21, 2: 22, 3: 4, 4: 5, 5: 24, 6: 20, 7: 23, 8: 6, 
    9: 10, 10: 11, 11: 12, 12: 13, 13: 14, 14: 15, 15: 16, 
    16: 17, 17: 18, 18: 19, 19: 7, 20: 8, 21: 9, 22: 1, 23: 3, 24: 2
}

DATA_DIR = "vhi_data_files"

def download_and_clean_data():
    if not os.path.exists(DATA_DIR):
        os.makedirs(DATA_DIR)
    
    all_dfs = []
    progress_bar = st.progress(0)

    for idx, (ukr_id, name) in enumerate(REGIONS.items()):
        noaa_id = UKR_TO_NOAA[ukr_id]
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        file_path = os.path.join(DATA_DIR, f"vhi_id_{ukr_id}_{timestamp}.csv")
        
        existing = [f for f in os.listdir(DATA_DIR) if f.startswith(f"vhi_id_{ukr_id}_")]
        if existing:
            file_path = os.path.join(DATA_DIR, existing[0])
        else:
            url = f"https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_admin.php?country=UKR&provinceID={noaa_id}&year1=1981&year2=2024&type=Mean"
            try:
                headers = {'User-Agent': 'Mozilla/5.0'}
                req = urllib.request.Request(url, headers=headers)
                with urllib.request.urlopen(req, timeout=30) as response:
                    content = response.read().decode('utf-8')
                    with open(file_path, 'w') as f:
                        f.write(content)
                time.sleep(0.5)
            except:
                continue

        try:
            with open(file_path, 'r') as f:
                raw_text = f.read()
            
            clean_text = raw_text.replace('<tt><pre>', '').replace('</pre></tt>', '').replace('<br>', '')
            lines = clean_text.split('\n')
            
            data_rows = []
            for line in lines:
                parts = [p.strip() for p in line.split(',') if p.strip()]
                if len(parts) >= 7 and parts[0].isdigit():
                    data_rows.append([parts[0], parts[1], parts[4], parts[5], parts[6]])
            
            if data_rows:
                df_temp = pd.DataFrame(data_rows, columns=['year', 'week', 'VCI', 'TCI', 'VHI'])
                for col in df_temp.columns:
                    df_temp[col] = pd.to_numeric(df_temp[col], errors='coerce')
                
                df_temp = df_temp.dropna()
                df_temp['region_id'] = ukr_id
                df_temp['region_name'] = name
                all_dfs.append(df_temp)
        except Exception as e:
            st.error(f"Помилка парсингу {name}: {e}")
            
        progress_bar.progress((idx + 1) / 24)

    return pd.concat(all_dfs, ignore_index=True) if all_dfs else None

st.set_page_config(layout="wide", page_title="VHI Analysis Ukraine")

if 'main_df' not in st.session_state:
    st.session_state.main_df = None

if st.session_state.main_df is None:
    st.title("🌾 Аналіз вегетаційних індексів України (Лабораторна №5)")
    
    if st.button("🚀 Завантажити дані з сервера NOAA"):
        with st.spinner("Зачекайте, завантаження 24 областей може тривати до 1 хвилини..."):
            st.session_state.main_df = download_and_clean_data()
            if st.session_state.main_df is not None:
                st.rerun()
            else:
                st.error("❌ Не вдалося завантажити дані. Перевірте підключення до інтернету.")
    st.stop()

df = st.session_state.main_df

if df is not None:
    st.title("🌾 Аналіз вегетаційних індексів України (Лабораторна №5)")

    col_sidebar, col_main = st.columns([1, 3])

    with col_sidebar:
        st.subheader("🔧 Налаштування")
        
        index_choice = st.selectbox("Оберіть індекс:", ["VHI", "VCI", "TCI"])
        region_choice = st.selectbox("Оберіть область:", sorted(df['region_name'].unique()))
        
        years = st.slider("Роки:", int(df.year.min()), int(df.year.max()), (2010, 2024))
        weeks = st.slider("Тижні:", 1, 52, (1, 52))
        
        sort_asc = st.checkbox("Сортувати за зростанням")
        sort_desc = st.checkbox("Сортувати за спаданням")
        
        if sort_asc and sort_desc:
            st.warning("Обрано обидва напрямки. Використано сортування за зростанням.")
            sort_desc = False
        
        if st.button("🔄 Reset (Скинути все)", type="primary", use_container_width=True):
            st.rerun()

    filtered_df = df[
        (df.year >= years[0]) & (df.year <= years[1]) &
        (df.week >= weeks[0]) & (df.week <= weeks[1])
    ]
    
    region_data = filtered_df[filtered_df.region_name == region_choice].copy()
    
    if sort_asc:
        region_data = region_data.sort_values(by=index_choice, ascending=True)
    elif sort_desc:
        region_data = region_data.sort_values(by=index_choice, ascending=False)

    with col_main:
        tab_table, tab_chart, tab_compare = st.tabs(["📋 Таблиця", "📈 Графік ряду", "📊 Порівняння"])

        with tab_table:
            col1, col2, col3, col4 = st.columns(4)
            with col1:
                st.metric("Середнє", f"{region_data[index_choice].mean():.2f}")
            with col2:
                st.metric("Медіана", f"{region_data[index_choice].median():.2f}")
            with col3:
                st.metric("Максимум", f"{region_data[index_choice].max():.2f}")
            with col4:
                st.metric("Мінімум", f"{region_data[index_choice].min():.2f}")
            
            st.dataframe(region_data[['year', 'week', index_choice]], use_container_width=True)
            
            csv = region_data[['year', 'week', index_choice]].to_csv(index=False).encode('utf-8')
            st.download_button("💾 Завантажити CSV", csv, f"{region_choice}_{index_choice}.csv", "text/csv")

        with tab_chart:
            fig, ax = plt.subplots(figsize=(12, 5))
            ax.plot(region_data['week'], region_data[index_choice], 'b-', linewidth=1.5, label=region_choice)
            
            if len(region_data) >= 5:
                rolling = region_data[index_choice].rolling(5, min_periods=1).mean()
                ax.plot(region_data['week'], rolling, 'r--', linewidth=2, label='Ковзне середнє (5)')
            
            ax.set_xlabel('Тиждень')
            ax.set_ylabel(index_choice)
            ax.set_title(f"{index_choice} для {region_choice} ({years[0]}-{years[1]})")
            ax.set_ylim(0, 100)
            ax.legend()
            ax.grid(True, alpha=0.3)
            ax.axhline(y=50, color='gray', linestyle=':', alpha=0.5)
            st.pyplot(fig)
            plt.close(fig)

        with tab_compare:
            comp_df = filtered_df.groupby('region_name')[index_choice].mean().reset_index()
            comp_df = comp_df.sort_values(index_choice, ascending=False)
            
            fig2, ax2 = plt.subplots(figsize=(12, 5))
            colors = ['coral' if r == region_choice else 'teal' for r in comp_df['region_name']]
            bars = ax2.bar(comp_df['region_name'], comp_df[index_choice], color=colors, edgecolor='black')
            
            for bar, val in zip(bars, comp_df[index_choice]):
                ax2.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 1, f'{val:.1f}', 
                        ha='center', va='bottom', fontsize=8)
            
            ax2.set_xticklabels(comp_df['region_name'], rotation=45, ha='right', fontsize=9)
            ax2.set_ylabel(f'Середній {index_choice}')
            ax2.set_title(f'Порівняння {index_choice} між областями')
            ax2.set_ylim(0, 100)
            ax2.grid(axis='y', alpha=0.3)
            plt.tight_layout()
            st.pyplot(fig2)
            plt.close(fig2)
            
            st.dataframe(comp_df, use_container_width=True)

    st.caption(f"📅 Оновлено: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")

else:
    st.error("❌ Не вдалося завантажити дані.")