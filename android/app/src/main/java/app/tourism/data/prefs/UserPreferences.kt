package app.tourism.data.prefs

import android.content.Context
import android.content.SharedPreferences
import androidx.core.content.edit
import app.organicmaps.R

class UserPreferences(context: Context) {
    private var sharedPref: SharedPreferences =
        context.getSharedPreferences("user", Context.MODE_PRIVATE)

    val languages = listOf(
        Language(code = "ru", name = "Русский"),
        Language(code = "en", name = "English")
    )

    val themes = listOf(
        Theme(code = "dark", name = context.getString(R.string.dark_theme)),
        Theme(code = "light", name = context.getString(R.string.light_theme)),
    )

    fun getLanguage(): Language? {
        val languageCode = sharedPref.getString("language", null)
        return languages.firstOrNull() { it.code == languageCode }
    }

    fun setLanguage(value: String) = sharedPref.edit { putString("language", value) }

    fun getTheme(): Theme? {
        val themeCode = sharedPref.getString("theme", "")
        return themes.firstOrNull() { it.code == themeCode }
    }

    fun setTheme(value: String?) = sharedPref.edit { putString("theme", value) }

    fun getToken() = sharedPref.getString("token", "")
    fun setToken(value: String?) = sharedPref.edit { putString("token", value) }
}

data class Language(val code: String, val name: String)

data class Theme(val code: String, val name: String)